#include <string.h>
#include "./suites.h"


/******************************************************************************
 * pcr_string_new() test cases
 */


#define DESC_NEW_1 "pcr_string_new() can create an empty string"
static bool test_new_1(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        return test && !*test;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_NEW_2 "pcr_string_new() can create an ASCII string"
static bool test_new_2(pcr_exception ex)
{
    pcr_exception_try (x) {
        const char *expect = "Hello, world!";
        pcr_string *test = pcr_string_new(expect, x);

        return test && !strcmp(test, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_NEW_3 "pcr_string_new() can create a Unicode string"
static bool test_new_3(pcr_exception ex)
{
    pcr_exception_try (x) {
        const char *expect = "Привет, мир!";
        pcr_string *test = pcr_string_new(expect, x);

        return test && !strcmp(test, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_NEW_4 "pcr_string_new() throws PCR_EXCEPTION_HANDLE if passed a" \
                   " NULL pointer for @cstr"
static bool test_new_4(pcr_exception ex)
{
    pcr_exception_try (x) {
        (void) pcr_string_new(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_copy() test cases
 */


#define DESC_COPY_1 "pcr_string_copy() can copy an empty string"
static bool test_copy_1(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        pcr_string *copy = pcr_string_copy(test, x);

        return copy && !*copy;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_COPY_2 "pcr_string_copy() can copy an ASCII string"
static bool test_copy_2(pcr_exception ex)
{
    pcr_exception_try (x) {
        const char *expect = "Hello, world!";
        pcr_string *test = pcr_string_new(expect, x);
        pcr_string *copy = pcr_string_copy(test, x);

        return test && !strcmp(copy, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_COPY_3 "pcr_string_copy() can copy a Unicode string"
static bool test_copy_3(pcr_exception ex)
{
    pcr_exception_try (x) {
        const char *expect = "Привет, мир!";
        pcr_string *test = pcr_string_new(expect, x);
        pcr_string *copy = pcr_string_copy(test, x);

        return test && !strcmp(copy, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_COPY_4 "pcr_string_copy() throws PCR_EXCEPTION_HANDLE if passed a" \
                   " NULL pointer for @ctx"
static bool test_copy_4(pcr_exception ex)
{
    pcr_exception_try (x) {
        (void) pcr_string_copy(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_len() test cases
 */


#define DESC_LEN_1 "pcr_string_len() reports 0 for a null string"
static bool test_len_1(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        return pcr_string_len(test, x) == 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_LEN_2 "pcr_string_len() reports the length of an ASCII string"
static bool test_len_2(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        return pcr_string_len(test, x) == 13;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_LEN_3 "pcr_string_len() reports the length of a Unicode string"
static bool test_len_3(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        return pcr_string_len(test, x) == 12;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_LEN_4 "pcr_string_len() throws PCR_EXCEPTION_HANDLE if passed a" \
                   " NULL pointer for @ctx"
static bool test_len_4(pcr_exception ex)
{
    pcr_exception_try (x) {
        (void) pcr_string_len(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_cmp() test cases
 */


#define DESC_CMP_1 "pcr_string_cmp() detects two equal null strings"
static bool test_cmp_1(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("", x);
        pcr_string *rhs = pcr_string_new("", x);

        return !pcr_string_cmp(lhs, rhs, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_CMP_2 "pcr_string_cmp() detects two equal ASCII strings"
static bool test_cmp_2(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("Hello, world!", x);
        pcr_string *rhs = pcr_string_new("Hello, world!", x);

        return !pcr_string_cmp(lhs, rhs, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_CMP_3 "pcr_string_cmp() detects two equal Unicode strings"
static bool test_cmp_3(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("Привет, мир!", x);
        pcr_string *rhs = pcr_string_new("Привет, мир!", x);

        return !pcr_string_cmp(lhs, rhs, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_CMP_4 "pcr_string_cmp() detects two unequal ASCII strings"
static bool test_cmp_4(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("Hello, world!", x);
        pcr_string *rhs = pcr_string_new("Goodbye, moon?", x);

        return pcr_string_cmp(lhs, rhs, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_CMP_5 "pcr_string_cmp() detects two unequal Unicode strings"
static bool test_cmp_5(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("Привет, мир!", x);
        pcr_string *rhs = pcr_string_new("До свидания, луна?", x);

        return pcr_string_cmp(lhs, rhs, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_CMP_6 "pcr_string_cmp() detects a lexicographically smaller" \
                   " ASCII string"
static bool test_cmp_6(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("Goodbye, moon?", x);
        pcr_string *rhs = pcr_string_new("Hello, world!", x);

        return pcr_string_cmp(lhs, rhs, x) < 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_CMP_7 "pcr_string_cmp() detects a lexicographically smaller" \
                   " Unicode string"
static bool test_cmp_7(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("До свидания, луна?", x);
        pcr_string *rhs = pcr_string_new("Привет, мир!", x);

        return pcr_string_cmp(lhs, rhs, x) < 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_CMP_8 "pcr_string_cmp() detects a lexicographically greater" \
                   " ASCII string"
static bool test_cmp_8(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("Goodbye, moon?", x);
        pcr_string *rhs = pcr_string_new("Hello, world!", x);

        return pcr_string_cmp(rhs, lhs, x) > 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_CMP_9 "pcr_string_cmp() detects a lexicographically greater" \
                   " Unicode string"
static bool test_cmp_9(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("До свидания, луна?", x);
        pcr_string *rhs = pcr_string_new("Привет, мир!", x);

        return pcr_string_cmp(rhs, lhs, x) > 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_CMP_10 "pcr_string_cmp() throws PCR_EXCEPTION_HANDLE if passed a" \
                   " NULL pointer for @lhs"
static bool test_cmp_10(pcr_exception ex)
{
    pcr_exception_try (x) {
        (void) pcr_string_cmp(NULL, "Hello, world!", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_CMP_11 "pcr_string_cmp() throws PCR_EXCEPTION_HANDLE if passed a" \
                   " NULL pointer for @rhs"
static bool test_cmp_11(pcr_exception ex)
{
    pcr_exception_try (x) {
        (void) pcr_string_cmp("Hello, world!", NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_add() test cases
 */


#define DESC_ADD_1 "pcr_string_add() adds two null strings"
static bool test_add_1(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        test = pcr_string_add(test, "", x);

        return !*test;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_ADD_2 "pcr_string_add() adds two ASCII strings"
static bool test_add_2(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello", x);
        test = pcr_string_add(test, ", ", x);
        test = pcr_string_add(test, "world", x);
        test = pcr_string_add(test, "!", x);

        return !strcmp(test, "Hello, world!");
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_ADD_3 "pcr_string_add() adds two Unicode strings"
static bool test_add_3(pcr_exception ex)
{
    pcr_exception_try (x) {
        const char *expect = "До свидания, луна?";

        pcr_string *test = pcr_string_new("До свидания", x);
        test = pcr_string_add(test, ", ", x);
        test = pcr_string_add(test, "луна", x);
        test = pcr_string_add(test, "?", x);

        return !strcmp(test, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}

#define DESC_ADD_4 "pcr_string_add() throws PCR_EXCEPTION_HANDLE if passed a" \
                   " NULL pointer for @ctx"
static bool test_add_4(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello", x);
        (void) test;

        test = pcr_string_add(NULL, ", ", x);
        (void) test;
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}



#define DESC_ADD_5 "pcr_string_add() throws PCR_EXCEPTION_HANDLE if passed a" \
                   " NULL pointer for @add"
static bool test_add_5(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello", x);
        test = pcr_string_add(test, NULL, x);
        (void) test;
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
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
        tc = pcr_testcase_new(&test_new_4, DESC_NEW_4, x);
        pcr_testsuite_push(ts, tc, x);

        tc = pcr_testcase_new(&test_copy_1, DESC_COPY_1, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_copy_2, DESC_COPY_2, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_copy_3, DESC_COPY_3, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_copy_4, DESC_COPY_4, x);
        pcr_testsuite_push(ts, tc, x);

        tc = pcr_testcase_new(&test_len_1, DESC_LEN_1, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_len_2, DESC_LEN_2, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_len_3, DESC_LEN_3, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_len_4, DESC_LEN_4, x);
        pcr_testsuite_push(ts, tc, x);

        tc = pcr_testcase_new(&test_cmp_1, DESC_CMP_1, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_cmp_2, DESC_CMP_2, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_cmp_3, DESC_CMP_3, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_cmp_4, DESC_CMP_4, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_cmp_5, DESC_CMP_5, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_cmp_6, DESC_CMP_6, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_cmp_7, DESC_CMP_7, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_cmp_8, DESC_CMP_9, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_cmp_9, DESC_CMP_9, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_cmp_10, DESC_CMP_10, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_cmp_11, DESC_CMP_11, x);
        pcr_testsuite_push(ts, tc, x);

        tc = pcr_testcase_new(&test_add_1, DESC_ADD_1, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_add_2, DESC_ADD_2, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_add_3, DESC_ADD_3, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_add_4, DESC_ADD_4, x);
        pcr_testsuite_push(ts, tc, x);
        tc = pcr_testcase_new(&test_add_5, DESC_ADD_5, x);
        pcr_testsuite_push(ts, tc, x);

        return ts;
    }

    pcr_exception_unwind(ex);
    return NULL;
}

