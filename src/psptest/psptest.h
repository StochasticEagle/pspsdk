#ifndef PSPTEST_H
#define PSPTEST_H

#include <stddef.h>
#include <psptypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PSPTEST_MODULE_ABI_VERSION 1u
#define PSPTEST_OUTPUT_PATH_MAX 320

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

typedef struct PspTestModuleControl {
    SceSize size;
    unsigned int version;
    SceUID completion_sema;
    volatile SceUID test_thread;
    volatile int state;
    volatile int result;
    char output_path[PSPTEST_OUTPUT_PATH_MAX];
} PspTestModuleControl;

typedef struct PspTestModuleStart {
    SceSize size;
    unsigned int version;
    PspTestModuleControl *control;
} PspTestModuleStart;

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
int psptest_start_module(SceSize arglen, void *argp, const char *suite, const PspTestCase *cases, size_t case_count);
int psptest_stop_module(void);

#define PSPTEST_JOIN_INNER(a, b) a##b
#define PSPTEST_JOIN(a, b) PSPTEST_JOIN_INNER(a, b)
#define PSPTEST_COVERS(symbol) static const char PSPTEST_JOIN(psptest_coverage_, __LINE__)[] __attribute__((used, section(".psptest_coverage"))) = #symbol
#define PSPTEST_TEST(name) static void name(PspTestContext *test)
#define PSPTEST_CASE(name) { #name, name, PSPTEST_FLAG_NONE }
#define PSPTEST_INTERACTIVE_CASE(name) { #name, name, PSPTEST_FLAG_INTERACTIVE }
#define PSPTEST_ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#define PSPTEST_MAIN(suite_name, cases_array) int main(int argc, char **argv) { return psptest_run_suite(argc, argv, suite_name, cases_array, PSPTEST_ARRAY_COUNT(cases_array)); }
#define PSPTEST_MODULE(suite_name, cases_array) \
    int module_start(SceSize args, void *argp) { return psptest_start_module(args, argp, suite_name, cases_array, PSPTEST_ARRAY_COUNT(cases_array)); } \
    int module_stop(SceSize args, void *argp) { (void)args; (void)argp; return psptest_stop_module(); }

#define PSPTEST_ASSERT_TRUE(test, expression) do { (test)->assertions++; if (!(expression)) { psptest_fail((test), __FILE__, __LINE__, "assertion failed: " #expression); return; } } while (0)
#define PSPTEST_ASSERT_EQ_INT(test, expected, actual) do { long long psptest_expected = (long long)(expected); long long psptest_actual = (long long)(actual); (test)->assertions++; if (psptest_expected != psptest_actual) { psptest_failf((test), __FILE__, __LINE__, "expected %lld, got %lld", psptest_expected, psptest_actual); return; } } while (0)
#define PSPTEST_ASSERT_NOT_NULL(test, pointer) do { const void *psptest_pointer = (const void *)(pointer); (test)->assertions++; if (psptest_pointer == NULL) { psptest_fail((test), __FILE__, __LINE__, "expected non-null pointer: " #pointer); return; } } while (0)

#ifdef __cplusplus
}
#endif

#endif
