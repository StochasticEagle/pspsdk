#ifndef PSPTEST_H
#define PSPTEST_H

#include <stddef.h>
#include <stdint.h>
#include <pspkerneltypes.h>
#include <pspthreadman.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PSPTEST_MODULE_ABI_VERSION 3u
#define PSPTEST_OUTPUT_PATH_MAX 320
#define PSPTEST_CASE_NAME_MAX 96
#define PSPTEST_MODULE_ARG_MAX 19

typedef enum PspTestStatus {
    PSPTEST_STATUS_PASS = 0,
    PSPTEST_STATUS_FAIL = 1,
    PSPTEST_STATUS_SKIP = 2,
    PSPTEST_STATUS_INTERACTIVE_PASS = 3,
    PSPTEST_STATUS_INTERACTIVE_FAIL = 4
} PspTestStatus;

typedef enum PspTestModuleState {
    PSPTEST_MODULE_IDLE = 0,
    PSPTEST_MODULE_RUNNING = 1,
    PSPTEST_MODULE_COMPLETE = 2,
    PSPTEST_MODULE_ERROR = 3
} PspTestModuleState;

typedef struct PspTestContext {
    const char *suite;
    const char *name;
    unsigned int assertions;
    unsigned int interactive_recorded;
    PspTestStatus status;
    char message[256];
} PspTestContext;

typedef void (*PspTestFunction)(PspTestContext *test);

typedef struct PspTestCase {
    const char *name;
    PspTestFunction function;
    unsigned int flags;
} PspTestCase;

typedef struct PspTestProgress {
    volatile unsigned int sequence;
    volatile SceUID test_thread;
    volatile int state;
    volatile int result;
    volatile int current_case;
    volatile unsigned int case_count;
    volatile unsigned int completed;
    volatile unsigned int passed;
    volatile unsigned int failed;
    volatile unsigned int skipped;
    volatile int previous_status;
    volatile uint64_t suite_start_us;
    volatile uint64_t case_start_us;
    volatile uint64_t completed_time_us;
    char current_case_name[PSPTEST_CASE_NAME_MAX];
    char previous_case_name[PSPTEST_CASE_NAME_MAX];
} PspTestProgress;

typedef struct PspTestModuleControl {
    SceSize size;
    unsigned int version;
    SceUID completion_sema;
    char output_path[PSPTEST_OUTPUT_PATH_MAX];
    PspTestProgress progress;
} PspTestModuleControl;

enum {
    PSPTEST_FLAG_NONE = 0u,
    PSPTEST_FLAG_INTERACTIVE = 1u << 0
};

void psptest_fail(PspTestContext *test, const char *file, int line, const char *message);
void psptest_failf(PspTestContext *test, const char *file, int line, const char *format, ...);
void psptest_skip(PspTestContext *test, const char *message);
void psptest_interactive_result(PspTestContext *test, int passed, const char *message);
int psptest_run_suite_to_file(const char *output_path, const char *suite, const PspTestCase *cases, size_t case_count);
int psptest_run_suite(int argc, char **argv, const char *suite, const PspTestCase *cases, size_t case_count);
int psptest_run_module(int argc, char **argv, const char *suite, const PspTestCase *cases, size_t case_count);

extern void __libcglue_init(int argc, char *argv[]);
extern unsigned int sce_newlib_priority __attribute__((weak));
extern unsigned int sce_newlib_attribute __attribute__((weak));
extern unsigned int sce_newlib_stack_kb_size __attribute__((weak));
extern const char *sce_newlib_main_thread_name __attribute__((weak));

static inline int psptest_module_unpack_args(SceSize args, void *argp, char **argv, int argv_capacity) {
    char *bytes = (char *)argp;
    SceSize offset = 0;
    int argc = 0;

    if (bytes == NULL || argv == NULL || argv_capacity < 1) return -1;

    while (offset < args && argc + 1 < argv_capacity) {
        argv[argc++] = bytes + offset;
        while (offset < args && bytes[offset] != '\0') offset++;
        if (offset >= args) return -1;
        offset++;
    }

    if (offset != args) return -1;
    argv[argc] = NULL;
    return argc;
}

static inline int psptest_module_hex_digit(char value) {
    if (value >= '0' && value <= '9') return value - '0';
    if (value >= 'a' && value <= 'f') return value - 'a' + 10;
    if (value >= 'A' && value <= 'F') return value - 'A' + 10;
    return -1;
}

static inline PspTestModuleControl *psptest_module_control_from_argv(int argc, char **argv) {
    static const char prefix[] = "--psptest-control=";
    int index;

    for (index = 0; index < argc; index++) {
        const char *argument = argv[index];
        const char *scan;
        uintptr_t address = 0;
        int prefix_index = 0;
        int digits = 0;

        if (argument == NULL) continue;
        while (prefix[prefix_index] != '\0' && argument[prefix_index] == prefix[prefix_index]) prefix_index++;
        if (prefix[prefix_index] != '\0') continue;

        scan = argument + prefix_index;
        if (scan[0] == '0' && (scan[1] == 'x' || scan[1] == 'X')) scan += 2;
        while (*scan != '\0') {
            int digit = psptest_module_hex_digit(*scan++);
            if (digit < 0) {
                digits = 0;
                break;
            }
            address = (address << 4) | (uintptr_t)digit;
            digits++;
        }
        if (digits != 0) return (PspTestModuleControl *)address;
    }

    return NULL;
}

static inline void psptest_module_publish_start(PspTestModuleControl *control, size_t case_count) {
    PspTestProgress *progress;

    if (control == NULL) return;
    progress = &control->progress;

    progress->sequence++;
    __sync_synchronize();
    progress->test_thread = sceKernelGetThreadId();
    progress->state = PSPTEST_MODULE_RUNNING;
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
    __sync_synchronize();
    progress->sequence++;
}

#define PSPTEST_JOIN_INNER(a, b) a##b
#define PSPTEST_JOIN(a, b) PSPTEST_JOIN_INNER(a, b)
#define PSPTEST_COVERS(symbol) static const char PSPTEST_JOIN(psptest_coverage_, __LINE__)[] __attribute__((used, section(".psptest_coverage"))) = #symbol
#define PSPTEST_TEST(name) static void name(PspTestContext *test)
#define PSPTEST_CASE(name) { #name, name, PSPTEST_FLAG_NONE }
#define PSPTEST_INTERACTIVE_CASE(name) { #name, name, PSPTEST_FLAG_INTERACTIVE }
#define PSPTEST_ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#define PSPTEST_MAIN(suite_name, cases_array) int main(int argc, char **argv) { return psptest_run_suite(argc, argv, suite_name, cases_array, PSPTEST_ARRAY_COUNT(cases_array)); }
#define PSPTEST_DEFAULT_MODULE_HEAP_KB 1024
#define PSPTEST_MODULE_WITH_HEAP(suite_name, cases_array, heap_kb) \
    int sce_newlib_heap_kb_size = (heap_kb); \
    void _fini(void) {} \
    static int psptest_module_test_thread(SceSize args, void *argp) { \
        char *psptest_argv[PSPTEST_MODULE_ARG_MAX + 1]; \
        int psptest_argc = psptest_module_unpack_args(args, argp, psptest_argv, PSPTEST_MODULE_ARG_MAX + 1); \
        PspTestModuleControl *psptest_control = psptest_argc > 0 ? psptest_module_control_from_argv(psptest_argc, psptest_argv) : NULL; \
        if (psptest_control == NULL || psptest_control->size != sizeof(PspTestModuleControl) || psptest_control->version != PSPTEST_MODULE_ABI_VERSION) { sceKernelExitThread(2); return 2; } \
        psptest_module_publish_start(psptest_control, PSPTEST_ARRAY_COUNT(cases_array)); \
        __libcglue_init(psptest_argc, psptest_argv); \
        return psptest_run_module(psptest_argc, psptest_argv, suite_name, cases_array, PSPTEST_ARRAY_COUNT(cases_array)); \
    } \
    int module_start(SceSize args, void *argp) { \
        char *psptest_argv[PSPTEST_MODULE_ARG_MAX + 1]; \
        int psptest_argc = psptest_module_unpack_args(args, argp, psptest_argv, PSPTEST_MODULE_ARG_MAX + 1); \
        PspTestModuleControl *psptest_control = psptest_argc > 0 ? psptest_module_control_from_argv(psptest_argc, psptest_argv) : NULL; \
        int psptest_priority = &sce_newlib_priority != NULL ? (int)sce_newlib_priority : 32; \
        unsigned int psptest_attribute = &sce_newlib_attribute != NULL ? sce_newlib_attribute : PSP_THREAD_ATTR_USER; \
        unsigned int psptest_stack_size = (&sce_newlib_stack_kb_size != NULL ? sce_newlib_stack_kb_size : 256u) * 1024u; \
        const char *psptest_thread_name = &sce_newlib_main_thread_name != NULL ? sce_newlib_main_thread_name : "psptest-module"; \
        SceUID psptest_thread; \
        int psptest_result; \
        if (psptest_control == NULL || psptest_control->size != sizeof(PspTestModuleControl) || psptest_control->version != PSPTEST_MODULE_ABI_VERSION) return -1; \
        psptest_thread = sceKernelCreateThread(psptest_thread_name, psptest_module_test_thread, psptest_priority, psptest_stack_size, psptest_attribute, NULL); \
        if (psptest_thread < 0) return psptest_thread; \
        psptest_result = sceKernelStartThread(psptest_thread, args, argp); \
        if (psptest_result < 0) sceKernelDeleteThread(psptest_thread); \
        return psptest_result; \
    } \
    int module_stop(SceSize args, void *argp) { (void)args; (void)argp; return 0; }
#define PSPTEST_MODULE(suite_name, cases_array) PSPTEST_MODULE_WITH_HEAP(suite_name, cases_array, PSPTEST_DEFAULT_MODULE_HEAP_KB)

#define PSPTEST_ASSERT_TRUE(test, expression) do { (test)->assertions++; if (!(expression)) { psptest_fail((test), __FILE__, __LINE__, "assertion failed: " #expression); return; } } while (0)
#define PSPTEST_ASSERT_EQ_INT(test, expected, actual) do { long long psptest_expected = (long long)(expected); long long psptest_actual = (long long)(actual); (test)->assertions++; if (psptest_expected != psptest_actual) { psptest_failf((test), __FILE__, __LINE__, "expected %lld, got %lld", psptest_expected, psptest_actual); return; } } while (0)
#define PSPTEST_ASSERT_NOT_NULL(test, pointer) do { const void *psptest_pointer = (const void *)(pointer); (test)->assertions++; if (psptest_pointer == NULL) { psptest_fail((test), __FILE__, __LINE__, "expected non-null pointer: " #pointer); return; } } while (0)

#ifdef __cplusplus
}
#endif

#endif
