#include "psptest.h"

#include <pspthreadman.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef struct PspTestModuleRuntime {
    PspTestModuleControl *control;
    const char *suite;
    const PspTestCase *cases;
    size_t case_count;
    SceUID thread_id;
} PspTestModuleRuntime;

static PspTestModuleRuntime psptest_module_runtime;

static void psptest_copy_message(char *destination, size_t destination_size, const char *message) {
    size_t index = 0;

    if (destination_size == 0) {
        return;
    }

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

static const char *psptest_argument_value(int argc, char **argv, const char *name, const char *fallback) {
    int index;
    size_t name_length = strlen(name);

    for (index = 1; index < argc; index++) {
        if (strncmp(argv[index], name, name_length) == 0 && argv[index][name_length] == '=' && argv[index][name_length + 1] != '\0') {
            return argv[index] + name_length + 1;
        }
        if (strcmp(argv[index], name) == 0 && index + 1 < argc) {
            return argv[index + 1];
        }
    }

    return fallback;
}

void psptest_fail(PspTestContext *test, const char *file, int line, const char *message) {
    if (test == NULL) {
        return;
    }

    test->status = PSPTEST_STATUS_FAIL;
    snprintf(test->message, sizeof(test->message), "%s:%d: %s", file != NULL ? file : "?", line, message != NULL ? message : "failure");
}

void psptest_failf(PspTestContext *test, const char *file, int line, const char *format, ...) {
    char detail[160];
    va_list arguments;

    if (test == NULL) {
        return;
    }

    va_start(arguments, format);
    vsnprintf(detail, sizeof(detail), format != NULL ? format : "failure", arguments);
    va_end(arguments);
    psptest_fail(test, file, line, detail);
}

void psptest_skip(PspTestContext *test, const char *message) {
    if (test == NULL) {
        return;
    }

    test->status = PSPTEST_STATUS_SKIP;
    psptest_copy_message(test->message, sizeof(test->message), message != NULL ? message : "skipped");
}

void psptest_interactive_result(PspTestContext *test, int passed, const char *message) {
    if (test == NULL) {
        return;
    }

    test->interactive_recorded = 1;
    test->status = passed ? PSPTEST_STATUS_INTERACTIVE_PASS : PSPTEST_STATUS_INTERACTIVE_FAIL;
    psptest_copy_message(test->message, sizeof(test->message), message != NULL ? message : (passed ? "interactive test passed" : "interactive test failed"));
}

int psptest_run_suite_to_file(const char *output_path, const char *suite, const PspTestCase *cases, size_t case_count) {
    FILE *output;
    size_t index;
    unsigned int passed = 0;
    unsigned int failed = 0;
    unsigned int skipped = 0;

    if (output_path == NULL || output_path[0] == '\0' || suite == NULL || cases == NULL) {
        return 2;
    }

    output = fopen(output_path, "w");
    if (output == NULL) {
        fprintf(stderr, "PSPTEST: unable to open result file: %s\n", output_path);
        return 2;
    }

    fprintf(output, "PSPTEST\t1\n");
    fprintf(output, "SUITE\t%s\n", suite);

    for (index = 0; index < case_count; index++) {
        PspTestContext test = {0};
        const PspTestCase *test_case = &cases[index];

        test.suite = suite;
        test.name = test_case->name;
        test.status = PSPTEST_STATUS_PASS;

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

        fprintf(output, "CASE\t%s\t%s\tassertions=%u", psptest_status_name(test.status), test_case->name, test.assertions);
        if (test.message[0] != '\0') {
            char sanitized[sizeof(test.message)];
            psptest_copy_message(sanitized, sizeof(sanitized), test.message);
            fprintf(output, "\tmessage=%s", sanitized);
        }
        fputc('\n', output);
        printf("[%s] %s/%s\n", psptest_status_name(test.status), suite, test_case->name);
    }

    fprintf(output, "SUMMARY\tpass=%u\tfail=%u\tskip=%u\ttotal=%u\n", passed, failed, skipped, passed + failed + skipped);
    fclose(output);
    return failed == 0 ? 0 : 1;
}

int psptest_run_suite(int argc, char **argv, const char *suite, const PspTestCase *cases, size_t case_count) {
    const char *output_path = psptest_argument_value(argc, argv, "--psptest-output", "psptest-results.log");
    return psptest_run_suite_to_file(output_path, suite, cases, case_count);
}

static int psptest_module_thread(SceSize args, void *argp) {
    PspTestModuleControl *control = psptest_module_runtime.control;
    int result;

    (void)args;
    (void)argp;

    if (control == NULL) {
        return 2;
    }

    result = psptest_run_suite_to_file(control->output_path, psptest_module_runtime.suite, psptest_module_runtime.cases, psptest_module_runtime.case_count);
    control->result = result;
    control->state = PSPTEST_MODULE_COMPLETE;
    if (control->completion_sema >= 0) {
        sceKernelSignalSema(control->completion_sema, 1);
    }
    return result;
}

int psptest_start_module(SceSize arglen, void *argp, const char *suite, const PspTestCase *cases, size_t case_count) {
    const PspTestModuleStart *start;
    PspTestModuleControl *control;
    SceUID thread_id;
    int result;

    if (argp == NULL || arglen < sizeof(PspTestModuleStart) || suite == NULL || cases == NULL) {
        return -1;
    }

    start = (const PspTestModuleStart *)argp;
    if (start->size != sizeof(PspTestModuleStart) || start->version != PSPTEST_MODULE_ABI_VERSION || start->control == NULL) {
        return -1;
    }

    control = start->control;
    if (control->size != sizeof(PspTestModuleControl) || control->version != PSPTEST_MODULE_ABI_VERSION) {
        return -1;
    }

    memset(&psptest_module_runtime, 0, sizeof(psptest_module_runtime));
    psptest_module_runtime.control = control;
    psptest_module_runtime.suite = suite;
    psptest_module_runtime.cases = cases;
    psptest_module_runtime.case_count = case_count;

    control->state = PSPTEST_MODULE_RUNNING;
    control->result = 2;
    control->test_thread = -1;

    thread_id = sceKernelCreateThread("psptest-suite", psptest_module_thread, 0x20, 0x20000, PSP_THREAD_ATTR_USER, NULL);
    if (thread_id < 0) {
        control->state = PSPTEST_MODULE_ERROR;
        control->result = thread_id;
        sceKernelSignalSema(control->completion_sema, 1);
        return 0;
    }

    psptest_module_runtime.thread_id = thread_id;
    control->test_thread = thread_id;
    result = sceKernelStartThread(thread_id, 0, NULL);
    if (result < 0) {
        sceKernelDeleteThread(thread_id);
        psptest_module_runtime.thread_id = -1;
        control->test_thread = -1;
        control->state = PSPTEST_MODULE_ERROR;
        control->result = result;
        sceKernelSignalSema(control->completion_sema, 1);
    }

    return 0;
}

int psptest_stop_module(void) {
    SceUID thread_id = psptest_module_runtime.thread_id;

    if (thread_id > 0) {
        if (sceKernelDeleteThread(thread_id) < 0) {
            sceKernelTerminateDeleteThread(thread_id);
        }
    }

    memset(&psptest_module_runtime, 0, sizeof(psptest_module_runtime));
    psptest_module_runtime.thread_id = -1;
    return 0;
}
