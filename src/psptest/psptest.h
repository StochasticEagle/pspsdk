#ifndef PSPTEST_H
#define PSPTEST_H

#include <stddef.h>
#include <stdint.h>
#include <pspkerneltypes.h>
#include <pspthreadman.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PSPTEST_ABI_VERSION 5u
#define PSPTEST_MODULE_MAGIC 0x50535454u
#define PSPTEST_CASE_NAME_MAX 96

typedef enum PspTestStatus {
    PSPTEST_STATUS_PASS = 0,
    PSPTEST_STATUS_FAIL = 1,
    PSPTEST_STATUS_SKIP = 2,
    PSPTEST_STATUS_INTERACTIVE_PASS = 3,
    PSPTEST_STATUS_INTERACTIVE_FAIL = 4
} PspTestStatus;

typedef enum PspTestRunState {
    PSPTEST_RUN_IDLE = 0,
    PSPTEST_RUN_RUNNING = 1,
    PSPTEST_RUN_COMPLETE = 2,
    PSPTEST_RUN_ERROR = 3
} PspTestRunState;

typedef enum PspTestFailureKind {
    PSPTEST_FAILURE_NONE = 0,
    PSPTEST_FAILURE_MESSAGE = 1,
    PSPTEST_FAILURE_EQ_INT = 2
} PspTestFailureKind;

typedef struct PspTestContext {
    const char *suite;
    const char *name;
    unsigned int assertions;
    unsigned int interactive_recorded;
    PspTestStatus status;
    PspTestFailureKind failure_kind;
    const char *file;
    int line;
    const char *message;
    long long expected;
    long long actual;
} PspTestContext;

typedef void (*PspTestFunction)(PspTestContext *test);

typedef struct PspTestEnvironment {
    unsigned int version;
    const char *program_path;
    const char *root_path;
} PspTestEnvironment;

typedef int (*PspTestLifecycleFunction)(const PspTestEnvironment *environment);

typedef struct PspTestCase {
    const char *name;
    PspTestFunction function;
    unsigned int flags;
} PspTestCase;

typedef struct PspTestSuite {
    unsigned int version;
    const char *name;
    const PspTestCase *cases;
    size_t case_count;
    int thread_priority;
    unsigned int thread_stack_size;
    unsigned int thread_attributes;
    PspTestLifecycleFunction setup;
    PspTestLifecycleFunction teardown;
} PspTestSuite;

typedef struct PspTestModuleRequest {
    unsigned int magic;
    unsigned int version;
    SceSize size;
    const PspTestSuite **suite_out;
} PspTestModuleRequest;

typedef struct PspTestProgress {
    volatile unsigned int sequence;
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

enum {
    PSPTEST_FLAG_NONE = 0u,
    PSPTEST_FLAG_INTERACTIVE = 1u << 0
};

enum {
    PSPTEST_RESULT_PASS = 0,
    PSPTEST_RESULT_FAIL = 1,
    PSPTEST_RESULT_ERROR = -1
};

static inline void psptest_fail(PspTestContext *test, const char *file, int line, const char *message) {
    if (test == NULL) return;
    test->status = PSPTEST_STATUS_FAIL;
    test->failure_kind = PSPTEST_FAILURE_MESSAGE;
    test->file = file;
    test->line = line;
    test->message = message;
}

static inline void psptest_fail_eq_int(PspTestContext *test, const char *file, int line, long long expected, long long actual) {
    if (test == NULL) return;
    test->status = PSPTEST_STATUS_FAIL;
    test->failure_kind = PSPTEST_FAILURE_EQ_INT;
    test->file = file;
    test->line = line;
    test->message = "integer values differ";
    test->expected = expected;
    test->actual = actual;
}

static inline void psptest_skip(PspTestContext *test, const char *message) {
    if (test == NULL) return;
    test->status = PSPTEST_STATUS_SKIP;
    test->failure_kind = PSPTEST_FAILURE_MESSAGE;
    test->message = message;
}

static inline void psptest_interactive_result(PspTestContext *test, int passed, const char *message) {
    if (test == NULL) return;
    test->interactive_recorded = 1;
    test->status = passed ? PSPTEST_STATUS_INTERACTIVE_PASS : PSPTEST_STATUS_INTERACTIVE_FAIL;
    test->failure_kind = PSPTEST_FAILURE_MESSAGE;
    test->message = message;
}

void psptest_call_test_with_gp(unsigned int gp_value, PspTestFunction function, PspTestContext *test);
int psptest_call_lifecycle_with_gp(unsigned int gp_value, PspTestLifecycleFunction function, const PspTestEnvironment *environment);
int psptest_run_suite(const PspTestSuite *suite, const char *output_path, PspTestProgress *progress, unsigned int gp_value);

#define PSPTEST_JOIN_INNER(a, b) a##b
#define PSPTEST_JOIN(a, b) PSPTEST_JOIN_INNER(a, b)
#define PSPTEST_COVERS(symbol) static const char PSPTEST_JOIN(psptest_coverage_, __LINE__)[] __attribute__((used, section(".psptest_coverage"))) = #symbol
#define PSPTEST_TEST(name) static void name(PspTestContext *test)
#define PSPTEST_CASE(name) { #name, name, PSPTEST_FLAG_NONE }
#define PSPTEST_INTERACTIVE_CASE(name) { #name, name, PSPTEST_FLAG_INTERACTIVE }
#define PSPTEST_ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

#define PSPTEST_DEFINE_SUITE(symbol, suite_name, cases_array, thread_attr, setup_fn, teardown_fn) \
    const PspTestSuite symbol = { \
        PSPTEST_ABI_VERSION, suite_name, cases_array, PSPTEST_ARRAY_COUNT(cases_array), \
        32, 256u * 1024u, thread_attr, setup_fn, teardown_fn \
    }

#define PSPTEST_MODULE(suite_name, cases_array, thread_attr, setup_fn, teardown_fn) \
    __attribute__((noreturn)) void _exit(int status) { sceKernelExitThread(status); for (;;) {} } \
    static const PspTestSuite psptest_module_suite = { \
        PSPTEST_ABI_VERSION, suite_name, cases_array, PSPTEST_ARRAY_COUNT(cases_array), \
        32, 256u * 1024u, thread_attr, setup_fn, teardown_fn \
    }; \
    int module_start(SceSize args, void *argp) { \
        PspTestModuleRequest *request = (PspTestModuleRequest *)argp; \
        if (args != sizeof(PspTestModuleRequest) || request == NULL) return -1; \
        if (request->magic != PSPTEST_MODULE_MAGIC || request->version != PSPTEST_ABI_VERSION || request->size != sizeof(PspTestModuleRequest) || request->suite_out == NULL) return -2; \
        *request->suite_out = &psptest_module_suite; \
        __sync_synchronize(); \
        return 0; \
    } \
    int module_stop(SceSize args, void *argp) { (void)args; (void)argp; return 0; }

#define PSPTEST_ASSERT_TRUE(test, expression) do { \
    (test)->assertions++; \
    if (!(expression)) { psptest_fail((test), __FILE__, __LINE__, "assertion failed: " #expression); return; } \
} while (0)

#define PSPTEST_ASSERT_EQ_INT(test, expected_value, actual_value) do { \
    long long psptest_expected = (long long)(expected_value); \
    long long psptest_actual = (long long)(actual_value); \
    (test)->assertions++; \
    if (psptest_expected != psptest_actual) { psptest_fail_eq_int((test), __FILE__, __LINE__, psptest_expected, psptest_actual); return; } \
} while (0)

#define PSPTEST_ASSERT_NOT_NULL(test, pointer) do { \
    const void *psptest_pointer = (const void *)(pointer); \
    (test)->assertions++; \
    if (psptest_pointer == NULL) { psptest_fail((test), __FILE__, __LINE__, "expected non-null pointer: " #pointer); return; } \
} while (0)

#ifdef __cplusplus
}
#endif

#endif
