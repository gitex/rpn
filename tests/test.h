#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////

#define ANSI_COLOR_RED            "\033[31m"
#define ANSI_COLOR_GREEN          "\033[32m"
#define ANSI_COLOR_MAGENTA        "\033[35m"
#define ANSI_COLOR_BLUE           "\033[34m"
#define ANSI_COLOR_RESET          "\033[0m"

//////////////////////////////////////////////////////////

#define COLOR_TEXT(text, color) color text ANSI_COLOR_RESET

#define FAIL_PREFIX COLOR_TEXT("[FAIL]", ANSI_COLOR_RED)
#define PASS_PREFIX COLOR_TEXT("[PASS]", ANSI_COLOR_GREEN)

//////////////////////////////////////////////////////////

#define LOG_PREFIX COLOR_TEXT("[LOG ]", ANSI_COLOR_BLUE)

#ifdef DEBUG
    #define LOG_CONDITION(cond) \
        do { \
            printf(LOG_PREFIX " (" #cond ") => %d\n", cond); \
        } while(0)
#else
    #define LOG_CONDITION(cond) do { } while(0)
#endif

//////////////////////////////////////////////////////////

static int tests_run = 0;
static int tests_failed = 0;
static int asserts_failed = 0;

//////////////////////////////////////////////////////////

#define MAX_TESTS 1024
typedef struct {
    const char *name;
    void (*fn)(void);
} TestMeta;

static TestMeta tests[MAX_TESTS];
static size_t tests_count = 0;

//////////////////////////////////////////////////////////

#define Test(test_name)                                                                                                \
    __attribute__((unused)) static void (test_name)(void);                                                               \
    __attribute__((constructor))                                                                                       \
    static void register_##test_name(void) {                                                                           \
        if (tests_count >= MAX_TESTS) {                                                                                \
            fprintf(stderr, "Exceeded limit of tests (max %d)\n", MAX_TESTS);                                          \
            abort();                                                                                                   \
        }                                                                                                              \
        tests[tests_count++] = (TestMeta){.name = #test_name, .fn = (test_name)};                                          \
    }                                                                                                                  \
    static void (test_name)(void)

#define Assert(cond)                                                                                         \
    do {                                                                                                     \
        if (!(cond)) {                                                                                       \
            tests_failed++; \
            asserts_failed++; \
            fprintf(stderr, FAIL_PREFIX " [%s:%d] (", __FILE__, __LINE__); \
            fprintf(stderr, #cond ") => %s\n", ((cond) == 0 ? "false" : "true"));                                                              \
            return; \
        }                                                                                                    \
    } while(0)

#define Ok(c)  Assert((c))

#define RunTest(test_meta)                                                                                   \
    do {                                                                                                               \
        asserts_failed = 0; \
        (test_meta).fn();                                                                                                     \
        if (!asserts_failed) { \
            fprintf(stdout, PASS_PREFIX " %s", (test_meta).name);                                                    \
        } \
    } while (0)

#define RunTests()                                                                                                    \
    do {                                                                                                               \
        fprintf(stdout, "\nFound %lu tests...\n\n", tests_count); \
        for (size_t i = 0; i < tests_count; i++) {                                                                     \
            if (tests_count > 0) {                                                                                     \
                RunTest(tests[i]);                                                                           \
                tests_run++; \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

#define SummarizeTestsResult()                                                                                                 \
    do {                                                                                                               \
        printf("\n\n  === RESULT ===\n");                                                                                \
        printf("  Run: %d\n", tests_run);                                                                              \
        printf("  Passed: %d\n", tests_run - tests_failed);                                                            \
        printf("  Failed: %d\n", tests_failed);                                                                        \
        if (tests_failed == 0) {                                                                                       \
            printf(COLOR_TEXT("\n  Status: OK\n\n", ANSI_COLOR_GREEN));                                                  \
        } else {                                                                                                       \
            printf(COLOR_TEXT("\n  Status: FAIL\n\n", ANSI_COLOR_RED));                                                  \
        }                                                                                                              \
    } while (0)

#endif  // !TEST_H
