#include <string.h>
#include "./suites.h"


#define DESC_NEW_1 "pcr_string_new() can create an empty string"
static bool test_new_1(void)
{
    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        return test && !*test;
    }

    return false;
}


#define DESC_NEW_2 "pcr_string_new() can create an ASCII string"
static bool test_new_2(void)
{
    pcr_exception_try (x) {
        const char *expect = "Hello, world!";
        pcr_string *test = pcr_string_new(expect, x);

        return test && !strcmp(test, expect);
    }

    return false;
}


#define DESC_NEW_3 "pcr_string_new() can create a Unicode string"
static bool test_new_3(void)
{
    pcr_exception_try (x) {
        const char *expect = "Привет, мир!";
        pcr_string *test = pcr_string_new(expect, x);

        pcr_log_trace("expect = %s", expect);
        pcr_log_trace("test   = %s", test);
        return test && !strcmp(test, expect);
    }

    return false;
}



extern pcr_testsuite *
pcr_string_testsuite(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_testsuite *ts = pcr_testsuite_new("PCR String (pcr_string)", x);

        pcr_testcase *tc = pcr_testcase_new(&test_new_1, DESC_NEW_1, x);
        pcr_testsuite_push(ts, tc, x);

        tc = pcr_testcase_new(&test_new_2, DESC_NEW_2, x);
        pcr_testsuite_push(ts, tc, x);

        tc = pcr_testcase_new(&test_new_3, DESC_NEW_3, x);
        pcr_testsuite_push(ts, tc, x);

        return ts;
    }

    pcr_exception_unwind(ex);
    return NULL;
}

