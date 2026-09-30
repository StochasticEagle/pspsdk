#include "psptest.h"

#include <pspthreadman.h>

#include <stdio.h>
#include <string.h>

static void psptest_copy_message(char *destination, size_t destination_size, const char *message) {
    size_t index = 0;

    if (destination_size == 0) return;
    if (message == NULL) {
        destination[0] = '\0';
        return;
    }

    while (message[index] != '\0' && index + 1 < destination_size) {
        char value = message[index];
        destination[index] = (value == '\n' || value == '\r' || value == '\t') ? ' ' : value;
        index++;
    }
    destination[index] = '\0';
}

static const char *psptest_status_name(PspTestStatus status) {
    switch (status) {
        case PSPTEST_STATUS_PASS: return "PASS";
        case PSPTEST_STATUS_FAIL: return "FAIL";
        case PSPTEST_STATUS_SKIP: return "SKIP";
        case PSPTEST_STATUS_INTERACTIVE_PASS: return "INTERACTIVE_PASS";
        case PSPTEST_STATUS_INTERACTIVE_FAIL: return "INTERACTIVE_FAIL";
        default: return "FAIL";
    }
}

static void psptest_progress_begin(PspTestProgress *progress) {
    if (progress == NULL) return;
    progress->sequence++;
    __sync_synchronize();
}

static void psptest_progress_end(PspTestProgress *progress) {
    if (progress == NULL) return;
    __sync_synchronize();
    progress->sequence++;
}

static void psptest_progress_initialize(PspTestProgress *progress, size_t case_count) {
    if (progress == NULL) return;

    psptest_progress_begin(progress);
    progress->state = PSPTEST_RUN_RUNNING;
    progress->result = 2;
    progress->current_case = -1;
    progress->case_count = (unsigned int)case_count;
    progress->completed = 0;
    progress->passed = 0;
    progress->failed = 0;
    progress->skipped = 0;
    progress->previous_status = PSPTEST_STATUS_PASS;
    progress->suite_start_us = sceKernelGetSystemTimeWide();
    progress->case_start_us = 0;
    progress->completed_time_us = 0;
    progress->current_case_name[0] = '\0';
    progress->previous_case_name[0] = '\0';
    psptest_progress_end(progress);
}

static void psptest_progress_start_case(PspTestProgress *progress, size_t index, const char *name) {
    if (progress == NULL) return;

    psptest_progress_begin(progress);
    progress->current_case = (int)index;
    progress->case_start_us = sceKernelGetSystemTimeWide();
    snprintf(progress->current_case_name, sizeof(progress->current_case_name), "%s", name != NULL ? name : "");
    psptest_progress_end(progress);
}

static void psptest_progress_finish_case(PspTestProgress *progress, size_t index, const char *name, PspTestStatus status, unsigned int passed, unsigned int failed, unsigned int skipped, uint64_t case_elapsed_us) {
    if (progress == NULL) return;

    psptest_progress_begin(progress);
    progress->completed = (unsigned int)index + 1u;
    progress->passed = passed;
    progress->failed = failed;
    progress->skipped = skipped;
    progress->previous_status = (int)status;
    progress->completed_time_us += case_elapsed_us;
    snprintf(progress->previous_case_name, sizeof(progress->previous_case_name), "%s", name != NULL ? name : "");
    progress->current_case = -1;
    progress->case_start_us = 0;
    progress->current_case_name[0] = '\0';
    psptest_progress_end(progress);
}

static void psptest_progress_complete(PspTestProgress *progress, int result) {
    if (progress == NULL) return;

    psptest_progress_begin(progress);
    progress->result = result;
    progress->state = PSPTEST_RUN_COMPLETE;
    psptest_progress_end(progress);
}

int psptest_run_suite(const PspTestSuite *suite, const char *output_path, PspTestProgress *progress) {
    FILE *output;
    size_t index;
    unsigned int passed = 0;
    unsigned int failed = 0;
    unsigned int skipped = 0;

    if (suite == NULL || suite->version != PSPTEST_ABI_VERSION || suite->name == NULL || suite->cases == NULL || output_path == NULL || output_path[0] == '\0') {
        if (progress != NULL) {
            progress->state = PSPTEST_RUN_ERROR;
            progress->result = 2;
        }
        return 2;
    }

    output = fopen(output_path, "w");
    if (output == NULL) {
        if (progress != NULL) {
            progress->state = PSPTEST_RUN_ERROR;
            progress->result = 2;
        }
        return 2;
    }

    psptest_progress_initialize(progress, suite->case_count);
    fprintf(output, "PSPTEST\t1\n");
    fprintf(output, "SUITE\t%s\n", suite->name);

    for (index = 0; index < suite->case_count; index++) {
        PspTestContext test = {0};
        const PspTestCase *test_case = &suite->cases[index];
        uint64_t case_start_us;

        test.suite = suite->name;
        test.name = test_case->name;
        test.status = PSPTEST_STATUS_PASS;
        test.failure_kind = PSPTEST_FAILURE_NONE;

        case_start_us = sceKernelGetSystemTimeWide();
        psptest_progress_start_case(progress, index, test_case->name);

        if (test_case->function == NULL) {
            psptest_fail(&test, __FILE__, __LINE__, "test function is null");
        } else {
            test_case->function(&test);
        }

        if ((test_case->flags & PSPTEST_FLAG_INTERACTIVE) != 0u && !test.interactive_recorded && test.status == PSPTEST_STATUS_PASS) {
            psptest_skip(&test, "interactive result was not recorded");
        }

        if (test.status == PSPTEST_STATUS_FAIL || test.status == PSPTEST_STATUS_INTERACTIVE_FAIL) {
            failed++;
        } else if (test.status == PSPTEST_STATUS_SKIP) {
            skipped++;
        } else {
            passed++;
        }

        psptest_progress_finish_case(progress, index, test_case->name, test.status, passed, failed, skipped, sceKernelGetSystemTimeWide() - case_start_us);

        fprintf(output, "CASE\t%s\t%s\tassertions=%u", psptest_status_name(test.status), test_case->name, test.assertions);
        if (test.file != NULL) fprintf(output, "\tfile=%s\tline=%d", test.file, test.line);
        if (test.failure_kind == PSPTEST_FAILURE_EQ_INT) fprintf(output, "\texpected=%lld\tactual=%lld", test.expected, test.actual);
        if (test.message != NULL && test.message[0] != '\0') {
            char sanitized[256];
            psptest_copy_message(sanitized, sizeof(sanitized), test.message);
            fprintf(output, "\tmessage=%s", sanitized);
        }
        fputc('\n', output);
    }

    fprintf(output, "SUMMARY\tpass=%u\tfail=%u\tskip=%u\ttotal=%u\n", passed, failed, skipped, passed + failed + skipped);
    fclose(output);

    psptest_progress_complete(progress, failed == 0 ? 0 : 1);
    return failed == 0 ? 0 : 1;
}
