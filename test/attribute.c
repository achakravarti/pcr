#include <float.h>
#include <math.h>
#include "./suites.h"


/******************************************************************************
 * pcr_attribute_new() test cases
 */

static bool
test_new_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_new() can create a null attribute";

    pcr_exception_try (x) {
        pcr_attribute *test = pcr_attribute_new_null("key", x);

        PCR_ATTRIBUTE type = pcr_attribute_type(test, x);
        pcr_string *key = pcr_attribute_key(test, x);
        void *value = pcr_attribute_value(test, x);

        return type == PCR_ATTRIBUTE_NULL && !pcr_string_cmp(key, "key", x)
               && !value;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_new() can create an int attribute";

    pcr_exception_try (x) {
        const pcr_string *KEY = "foo";
        const int64_t VALUE = -1024;
        const PCR_ATTRIBUTE TYPE  = PCR_ATTRIBUTE_INT;
        pcr_attribute *test = pcr_attribute_new_int(KEY, VALUE, x);

        PCR_ATTRIBUTE type = pcr_attribute_type(test, x);
        pcr_string *key = pcr_attribute_key(test, x);
        int64_t *value = pcr_attribute_value(test, x);

        return type == TYPE && !pcr_string_cmp(key, KEY, x) && *value == VALUE;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_new() can create a float attribute";

    pcr_exception_try (x) {
        const pcr_string *KEY = "frobnoz";
        const double VALUE = -3.141592654;
        const PCR_ATTRIBUTE TYPE  = PCR_ATTRIBUTE_FLOAT;
        pcr_attribute *test = pcr_attribute_new_float(KEY, VALUE, x);

        PCR_ATTRIBUTE type = pcr_attribute_type(test, x);
        pcr_string *key = pcr_attribute_key(test, x);
        double *value = pcr_attribute_value(test, x);

        return type == TYPE && !pcr_string_cmp(key, KEY, x)
                       && fabs(*value - VALUE) < DBL_EPSILON;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_new() can create an ASCII string attribute";

    pcr_exception_try (x) {
        const pcr_string *KEY = "bar";
        const pcr_string *VALUE = "Hello, world!";
        const PCR_ATTRIBUTE TYPE  = PCR_ATTRIBUTE_TEXT;
        pcr_attribute *test = pcr_attribute_new_text(KEY, VALUE, x);

        PCR_ATTRIBUTE type = pcr_attribute_type(test, x);
        pcr_string *key = pcr_attribute_key(test, x);
        pcr_string *value = pcr_attribute_value(test, x);

        return type == TYPE && !pcr_string_cmp(key, KEY, x)
               && !pcr_string_cmp(value, VALUE, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_new() can create a Unicode string attribute";

    pcr_exception_try (x) {
        const pcr_string *KEY = "bar";
        const pcr_string *VALUE = "Привет, мир!";
        const PCR_ATTRIBUTE TYPE  = PCR_ATTRIBUTE_TEXT;
        pcr_attribute *test = pcr_attribute_new_text(KEY, VALUE, x);

        PCR_ATTRIBUTE type = pcr_attribute_type(test, x);
        pcr_string *key = pcr_attribute_key(test, x);
        pcr_string *value = pcr_attribute_value(test, x);

        return type == TYPE && !pcr_string_cmp(key, KEY, x)
               && !pcr_string_cmp(value, VALUE, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_new() throws PCR_EXCEPTION_STRING if passed a NULL"
            " pointer for @key";

    pcr_exception_try (x) {
        (void) pcr_attribute_new(PCR_ATTRIBUTE_NULL, NULL, NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_7(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_new() throws PCR_EXCEPTION_STRING if passed a null"
            " string for @key";

    pcr_exception_try (x) {
        (void) pcr_attribute_new(PCR_ATTRIBUTE_NULL, "", NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_8(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_new() does not throw PCR_EXCEPTION_HANDLE if passed"
            " a null pointer for @value when @field is PCR_ATTRIBUTE_NULL";

    pcr_exception_try (x) {
        (void) pcr_attribute_new(PCR_ATTRIBUTE_NULL, "foo", NULL, x);
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_9(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_new() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @value when @field is not PCR_ATTRIBUTE_NULL";

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
 * pcr_attribute_copy() test cases
 */


static bool
test_copy_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_copy() can copy a null attribute";

    pcr_exception_try (x) {
        pcr_attribute *test = pcr_attribute_new(PCR_ATTRIBUTE_NULL, "key",
                                                NULL, x);
        pcr_attribute *copy = pcr_attribute_copy(test, x);

        PCR_ATTRIBUTE type = pcr_attribute_type(copy, x);
        pcr_string *key = pcr_attribute_key(copy, x);
        void *value = pcr_attribute_value(copy, x);

        return type == PCR_ATTRIBUTE_NULL && !pcr_string_cmp(key, "key", x)
                       && !value;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_copy_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_copy() can copy an int attribute";

    pcr_exception_try (x) {
        const pcr_string *KEY = "foo";
        const int64_t VALUE = -1024;
        const PCR_ATTRIBUTE TYPE  = PCR_ATTRIBUTE_INT;
        pcr_attribute *test = pcr_attribute_new(TYPE, KEY, &VALUE, x);
        pcr_attribute *copy = pcr_attribute_copy(test, x);

        PCR_ATTRIBUTE type = pcr_attribute_type(copy, x);
        pcr_string *key = pcr_attribute_key(copy, x);
        int64_t *value = pcr_attribute_value(copy, x);

        return type == TYPE && !pcr_string_cmp(key, KEY, x) && *value == VALUE;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_copy_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_copy() can copy a float attribute";

    pcr_exception_try (x) {
        const pcr_string *KEY = "frobnoz";
        const double VALUE = -3.141592654;
        const PCR_ATTRIBUTE TYPE  = PCR_ATTRIBUTE_FLOAT;
        pcr_attribute *test = pcr_attribute_new(TYPE, KEY, &VALUE, x);
        pcr_attribute *copy = pcr_attribute_copy(test, x);

        PCR_ATTRIBUTE type = pcr_attribute_type(copy, x);
        pcr_string *key = pcr_attribute_key(copy, x);
        double *value = pcr_attribute_value(copy, x);

        return type == TYPE && !pcr_string_cmp(key, KEY, x)
                       && fabs(*value - VALUE) < DBL_EPSILON;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_copy_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_copy() can copy an ASCII string attribute";

    pcr_exception_try (x) {
        const pcr_string *KEY = "bar";
        const pcr_string *VALUE = "Hello, world!";
        const PCR_ATTRIBUTE TYPE  = PCR_ATTRIBUTE_TEXT;
        pcr_attribute *test = pcr_attribute_new(TYPE, KEY, VALUE, x);
        pcr_attribute *copy = pcr_attribute_copy(test, x);

        PCR_ATTRIBUTE type = pcr_attribute_type(copy, x);
        pcr_string *key = pcr_attribute_key(copy, x);
        pcr_string *value = pcr_attribute_value(copy, x);

        return type == TYPE && !pcr_string_cmp(key, KEY, x)
               && !pcr_string_cmp(value, VALUE, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_copy_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_copy() can copy a Unicode string attribute";

    pcr_exception_try (x) {
        const pcr_string *KEY = "bar";
        const pcr_string *VALUE = "Привет, мир!";
        const PCR_ATTRIBUTE TYPE  = PCR_ATTRIBUTE_TEXT;
        pcr_attribute *test = pcr_attribute_new(TYPE, KEY, VALUE, x);
        pcr_attribute *copy = pcr_attribute_copy(test, x);

        PCR_ATTRIBUTE type = pcr_attribute_type(copy, x);
        pcr_string *key = pcr_attribute_key(copy, x);
        pcr_string *value = pcr_attribute_value(copy, x);

        return type == TYPE && !pcr_string_cmp(key, KEY, x)
               && !pcr_string_cmp(value, VALUE, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_copy_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_copy() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_attribute_copy(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_attribute_key() test cases
 */


static bool
test_key_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_key() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_attribute_key(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_attribute_value() test cases
 */


static bool
test_value_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_value() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_attribute_value(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_attribute_type() test cases
 */


static bool
test_type_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_type() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_attribute_type(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_attribute_valuesz() test cases
 */


static bool
test_valuesz_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_valuesz() returns 0 for a null attribute";

    pcr_exception_try (x) {
        const PCR_ATTRIBUTE type = PCR_ATTRIBUTE_NULL;
        const pcr_string *key = "dummy";
        const void *value = NULL;

        pcr_attribute *test = pcr_attribute_new(type, key, value, x);
        return !pcr_attribute_valuesz(test, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_valuesz_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_valuesz() returns the size of an int attribute";

    pcr_exception_try (x) {
        const PCR_ATTRIBUTE type = PCR_ATTRIBUTE_INT;
        const pcr_string *key = "int_value";
        const int64_t value = -1024;

        pcr_attribute *test = pcr_attribute_new(type, key, &value, x);
        return pcr_attribute_valuesz(test, x) == sizeof value;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_valuesz_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_valuesz() returns the size of a float attribute";

    pcr_exception_try (x) {
        const PCR_ATTRIBUTE type = PCR_ATTRIBUTE_FLOAT;
        const pcr_string *key = "float_value";
        const double value = -3.141592654;

        pcr_attribute *test = pcr_attribute_new(type, key, &value, x);
        return pcr_attribute_valuesz(test, x) == sizeof value;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_valuesz_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_valuesz() returns the size of an ASCII text"
            " attribute";

    pcr_exception_try (x) {
        const pcr_string *key = "bar";
        const pcr_string *value = pcr_string_new("Hello, world!", x);
        const PCR_ATTRIBUTE type  = PCR_ATTRIBUTE_TEXT;

        pcr_attribute *test = pcr_attribute_new(type, key, value, x);
        return pcr_attribute_valuesz(test, x) == pcr_string_sz(value, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_valuesz_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_valuesz() returns the size of a Unicode text"
            " attribute";

    pcr_exception_try (x) {
        const pcr_string *key = "bar";
        const pcr_string *value = pcr_string_new("Привет, мир!", x);
        const PCR_ATTRIBUTE type  = PCR_ATTRIBUTE_TEXT;

        pcr_attribute *test = pcr_attribute_new(type, key, value, x);
        return pcr_attribute_valuesz(test, x) == pcr_string_sz(value, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_valuesz_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_valuesz() throws PCR_EXCEPTION_HANDLE if passed a"
            " null pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_attribute_valuesz(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_string_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_string() stringifies a null attribute";

    pcr_exception_try (x) {
        const PCR_ATTRIBUTE type = PCR_ATTRIBUTE_NULL;
        const pcr_string *key = "dummy";
        const void *value = NULL;

        pcr_attribute *test = pcr_attribute_new(type, key, value, x);
        return !pcr_string_cmp(pcr_attribute_string(test, x), "NULL", x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_string_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_string() stringifies an int attribute";

    pcr_exception_try (x) {
        const PCR_ATTRIBUTE type = PCR_ATTRIBUTE_INT;
        const pcr_string *key = "int_value";
        const int64_t value = -1024;

        pcr_attribute *test = pcr_attribute_new(type, key, &value, x);
        pcr_string *expect = pcr_string_int(value, x);
        return !pcr_string_cmp(pcr_attribute_string(test, x), expect, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_string_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_string() stringifies a float attribute";

    pcr_exception_try (x) {
        const PCR_ATTRIBUTE type = PCR_ATTRIBUTE_FLOAT;
        const pcr_string *key = "float_value";
        const double value = -3.141592654;

        pcr_attribute *test = pcr_attribute_new(type, key, &value, x);
        pcr_string *expect = pcr_string_float(value, x);
        return !pcr_string_cmp(pcr_attribute_string(test, x), expect, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_string_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_string() stringifies an ASCII text attribute";

    pcr_exception_try (x) {
        const pcr_string *key = "bar";
        const pcr_string *value = pcr_string_new("Hello, world!", x);
        const PCR_ATTRIBUTE type  = PCR_ATTRIBUTE_TEXT;

        pcr_attribute *test = pcr_attribute_new(type, key, value, x);
        return !pcr_string_cmp(pcr_attribute_string(test, x), value, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_string_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_string() stringifies a Unicode text attribute";

    pcr_exception_try (x) {
        const pcr_string *key = "bar";
        const pcr_string *value = pcr_string_new("Привет, мир!", x);
        const PCR_ATTRIBUTE type  = PCR_ATTRIBUTE_TEXT;

        pcr_attribute *test = pcr_attribute_new(type, key, value, x);
        return !pcr_string_cmp(pcr_attribute_string(test, x), value, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_string_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_string() throws PCR_EXCEPTION_HANDLE if passed a"
            " null pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_attribute_string(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_attribute_json() interface
 */

static bool
test_json_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_json() jsonifies a null attribute";

    pcr_exception_try (x) {
        const PCR_ATTRIBUTE type = PCR_ATTRIBUTE_NULL;
        const pcr_string *key = "dummy";
        const void *value = NULL;
        pcr_attribute *test = pcr_attribute_new(type, key, value, x);

        const pcr_string *expect = "\"dummy\":\"NULL\"";
        return !pcr_string_cmp(pcr_attribute_json(test, x), expect, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_json_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_json() jsonifies an int attribute";

    pcr_exception_try (x) {
        const PCR_ATTRIBUTE type = PCR_ATTRIBUTE_INT;
        const pcr_string *key = "int_value";
        const int64_t value = -1024;
        pcr_attribute *test = pcr_attribute_new(type, key, &value, x);

        const pcr_string *expect = "\"int_value\":\"-1024\"";
        return !pcr_string_cmp(pcr_attribute_json(test, x), expect, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_json_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_json() jsonifies a float attribute";

    pcr_exception_try (x) {
        const PCR_ATTRIBUTE type = PCR_ATTRIBUTE_FLOAT;
        const pcr_string *key = "float_value";
        const double value = -3.141592654;
        pcr_attribute *test = pcr_attribute_new(type, key, &value, x);

        const pcr_string *expect = "\"float_value\":\"-3.141593\"";
        return !pcr_string_cmp(pcr_attribute_json(test, x), expect, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_json_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_json() jsonifies an ASCII text attribute";

    pcr_exception_try (x) {
        const pcr_string *key = "bar";
        const pcr_string *value = pcr_string_new("Hello, world!", x);
        const PCR_ATTRIBUTE type  = PCR_ATTRIBUTE_TEXT;
        pcr_attribute *test = pcr_attribute_new(type, key, value, x);

        const pcr_string *expect = "\"bar\":\"Hello, world!\"";
        return !pcr_string_cmp(pcr_attribute_json(test, x), expect, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_json_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_json() jsonifies a Unicode text attribute";

    pcr_exception_try (x) {
        const pcr_string *key = "bar";
        const pcr_string *value = pcr_string_new("Привет, мир!", x);
        const PCR_ATTRIBUTE type  = PCR_ATTRIBUTE_TEXT;
        pcr_attribute *test = pcr_attribute_new(type, key, value, x);

        const pcr_string *expect = "\"bar\":\"Привет, мир!\"";
        return !pcr_string_cmp(pcr_attribute_json(test, x), expect, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_json_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_attribute_json() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_attribute_json(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_attribute_testsuite() interface
 */


static pcr_unittest *unit_tests[] = {
    test_new_1, test_new_2, test_new_3, test_new_4, test_new_5, test_new_6,
    test_new_7, test_new_8, test_new_9, test_copy_1, test_copy_2, test_copy_3,
    test_copy_4, test_copy_5, test_copy_6, test_key_1, test_value_1,
    test_type_1, test_valuesz_1, test_valuesz_2, test_valuesz_3, test_valuesz_4,
    test_valuesz_5, test_valuesz_6, test_string_1, test_string_2, test_string_3,
    test_string_4, test_string_5, test_string_6, test_json_1, test_json_2,
    test_json_3, test_json_4, test_json_5, test_json_6
};


extern pcr_testsuite *
pcr_attribute_testsuite(pcr_exception ex)
{
    pcr_exception_try (x) {
        const pcr_string *name = "PCR Attribute (pcr_attribute)";
        const size_t len = sizeof unit_tests / sizeof *unit_tests;

        return pcr_testsuite_new_2(name, unit_tests, len, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}

