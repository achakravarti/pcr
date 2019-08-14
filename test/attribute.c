#include <float.h>
#include <math.h>
#include "./suites.h"


/******************************************************************************
 * pcr_attribute_new() test cases
 */

#define DESC_NEW_1 "pcr_attribute_new() can create a null attribute"
static bool test_new_1(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_attribute *test = pcr_attribute_new(PCR_ATTRIBUTE_NULL, "key",
                                                NULL, x);

        PCR_ATTRIBUTE type = pcr_attribute_type(test, x);
        pcr_string *key = pcr_attribute_key(test, x);
        void *value = pcr_attribute_value(test, x);

        return type == PCR_ATTRIBUTE_NULL && !pcr_string_cmp(key, "key", x)
                       && !value;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_NEW_2 "pcr_attribute_new() can create an int attribute"
static bool test_new_2(pcr_exception ex)
{
    pcr_exception_try (x) {
        const pcr_string *KEY = "foo";
        const int64_t VALUE = -1024;
        const PCR_ATTRIBUTE TYPE  = PCR_ATTRIBUTE_INT;
        pcr_attribute *test = pcr_attribute_new(TYPE, KEY, &VALUE, x);

        PCR_ATTRIBUTE type = pcr_attribute_type(test, x);
        pcr_string *key = pcr_attribute_key(test, x);
        int64_t *value = pcr_attribute_value(test, x);

        return type == TYPE && !pcr_string_cmp(key, KEY, x) && *value == VALUE;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_NEW_3 "pcr_attribute_new() can create a float attribute"
static bool test_new_3(pcr_exception ex)
{
    pcr_exception_try (x) {
        const pcr_string *KEY = "frobnoz";
        const double VALUE = -3.141592654;
        const PCR_ATTRIBUTE TYPE  = PCR_ATTRIBUTE_FLOAT;
        pcr_attribute *test = pcr_attribute_new(TYPE, KEY, &VALUE, x);

        PCR_ATTRIBUTE type = pcr_attribute_type(test, x);
        pcr_string *key = pcr_attribute_key(test, x);
        double *value = pcr_attribute_value(test, x);

        return type == TYPE && !pcr_string_cmp(key, KEY, x)
                       && fabs(*value - VALUE) < DBL_EPSILON;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_NEW_4 "pcr_attribute_new() can create an ASCII string attribute"
static bool test_new_4(pcr_exception ex)
{
    pcr_exception_try (x) {
        const pcr_string *KEY = "bar";
        const pcr_string *VALUE = "Hello, world!";
        const PCR_ATTRIBUTE TYPE  = PCR_ATTRIBUTE_TEXT;
        pcr_attribute *test = pcr_attribute_new(TYPE, KEY, VALUE, x);

        PCR_ATTRIBUTE type = pcr_attribute_type(test, x);
        pcr_string *key = pcr_attribute_key(test, x);
        pcr_string *value = pcr_attribute_value(test, x);

        return type == TYPE && !pcr_string_cmp(key, KEY, x)
               && !pcr_string_cmp(value, VALUE, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_NEW_5 "pcr_attribute_new() throws PCR_EXCEPTION_STRING if passed" \
                   " a NULL pointer for @key"
static bool test_new_5(pcr_exception ex)
{
    pcr_exception_try (x) {
        (void) pcr_attribute_new(PCR_ATTRIBUTE_NULL, NULL, NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_NEW_6 "pcr_attribute_new() throws PCR_EXCEPTION_STRING if passed" \
                   " a null string for @key"
static bool test_new_6(pcr_exception ex)
{
    pcr_exception_try (x) {
        (void) pcr_attribute_new(PCR_ATTRIBUTE_NULL, "", NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_NEW_7 "pcr_attribute_new() does not throw PCR_EXCEPTION_HANDLE" \
                   " if passed a null pointer for @value when @field is"     \
                   " PCR_ATTRIBUTE_NULL"
static bool test_new_7(pcr_exception ex)
{
    pcr_exception_try (x) {
        (void) pcr_attribute_new(PCR_ATTRIBUTE_NULL, "foo", NULL, x);
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


#define DESC_NEW_8 "pcr_attribute_new() throws PCR_EXCEPTION_HANDLE if passed" \
                   " a null pointer for @value when @field is not            " \
                   " PCR_ATTRIBUTE_NULL"
static bool test_new_8(pcr_exception ex)
{
    pcr_exception_try (x) {
        (void) pcr_attribute_new(PCR_ATTRIBUTE_INT, "foo", NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind (ex);
    return false;
}


/******************************************************************************
 * pcr_attribute_testsuite() interface
 */


static pcr_unittest *unit_tests[] = {
    test_new_1, test_new_2, test_new_3, test_new_4, test_new_5, test_new_6,
    test_new_7, test_new_8
};

static const pcr_string *UNIT_DESCS[] = {
    DESC_NEW_1, DESC_NEW_2, DESC_NEW_3, DESC_NEW_4, DESC_NEW_5, DESC_NEW_6,
    DESC_NEW_7, DESC_NEW_8
};


extern pcr_testsuite *
pcr_attribute_testsuite(pcr_exception ex)
{
    pcr_exception_try (x) {
        const pcr_string *name = "PCR Attribute (pcr_attribute)";
        pcr_testsuite *ts = pcr_testsuite_new(name, x);

        register pcr_testcase *tc;
        register size_t len = sizeof unit_tests / sizeof *unit_tests;
        for (register size_t i = 0; i < len; i++) {
            tc = pcr_testcase_new(unit_tests[i], UNIT_DESCS[i], x);
            pcr_testsuite_push(ts, tc, x);
        }

        return ts;
    }

    pcr_exception_unwind(ex);
    return NULL;
}

