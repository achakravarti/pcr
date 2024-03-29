#include "./suites.h"


static const pcr_string *SAMPLE_NAME = "Test Resultset";

static const pcr_string *SAMPLE_KEYS[] = {"id", "fname", "lname", "attempts",
                                          "time"};

static const PCR_ATTRIBUTE SAMPLE_TYPES[] = {PCR_ATTRIBUTE_INT,
                                             PCR_ATTRIBUTE_TEXT,
                                             PCR_ATTRIBUTE_TEXT,
                                             PCR_ATTRIBUTE_INT,
                                             PCR_ATTRIBUTE_FLOAT};

static const size_t SAMPLE_LEN = sizeof SAMPLE_TYPES / sizeof *SAMPLE_TYPES;


static void
sample_row_push(pcr_resultset **ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        const pcr_attribute *arr[] = {
            pcr_attribute_new_int("id", 1024, x),
            pcr_attribute_new_text("fname", "John", x),
            pcr_attribute_new_text("lname", "Вороно́й", x),
            pcr_attribute_new_int("attempts", -111, x),
            pcr_attribute_new_float("time", 123.456789, x)
        };
        const size_t len = sizeof arr / sizeof *arr;

        pcr_resultset_push_2(ctx, pcr_attribute_vector_new_2(arr, len, x), x);
    }

    pcr_exception_unwind(ex);
}


static bool
sample_row_match(const pcr_resultset *ctx, pcr_exception ex)
{
    const size_t row = 1;
    const pcr_string *values[] = {
        "1024", "John", "Вороно́й", "-111", "123.456789"
    };

    pcr_exception_try (x) {
        register pcr_attribute *attr;
        register pcr_string *key, *value;
        register PCR_ATTRIBUTE type;

        for (register size_t i = 1; i <= SAMPLE_LEN; i++) {
            attr = pcr_resultset_attrib(ctx, row, i, x);
            type = pcr_attribute_type(attr, x);
            key = pcr_attribute_key(attr, x);
            value = pcr_attribute_string(attr, x);

            if (pcr_string_cmp(key, SAMPLE_KEYS[i - 1], x)
                || type != SAMPLE_TYPES[i - 1]
                || pcr_string_cmp(value, values[i - 1], x))
                return false;
        }

        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
sample_types_match(const PCR_ATTRIBUTE_VECTOR *vec, pcr_exception ex)
{
    pcr_exception_try (x) {
        register size_t len = PCR_ATTRIBUTE_VECTOR_LEN(vec, x);
        if (len != SAMPLE_LEN)
            return false;

        register PCR_ATTRIBUTE type;
        for (register size_t i = 1; i <= len; i++) {
            type = PCR_ATTRIBUTE_VECTOR_ELEM(vec, i, x);
            if (type != SAMPLE_TYPES[i - 1])
                return false;
        }

        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
sample_keys_match(const pcr_string_vector *vec, pcr_exception ex)
{
    pcr_exception_try (x) {
        register size_t len = pcr_string_vector_len(vec, x);
        if (len != SAMPLE_LEN)
            return false;

        register pcr_string *key;
        for (register size_t i = 1; i <= len; i++) {
            key = pcr_string_vector_elem(vec, i, x);
            if (pcr_string_cmp(key, SAMPLE_KEYS[i - 1], x))
                return false;
        }

        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_resultset_new_2() test cases
 */


static bool
new_2_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new_2() creates a new result set";

    pcr_exception_try (x) {
        pcr_resultset *rs = pcr_resultset_new_2(SAMPLE_NAME, SAMPLE_KEYS,
                                                SAMPLE_TYPES, SAMPLE_LEN, x);

        return !pcr_resultset_rows(rs, x)
               && pcr_resultset_cols(rs, x) == SAMPLE_LEN
               && sample_types_match(pcr_resultset_types(rs, x), x)
               && sample_keys_match(pcr_resultset_keys(rs, x), x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
new_2_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new_2() creates a new result set with a reference"
            " count of 1";

    pcr_exception_try (x) {
        pcr_resultset *rs = pcr_resultset_new_2(SAMPLE_NAME, SAMPLE_KEYS,
                                                SAMPLE_TYPES, SAMPLE_LEN, x);

        return pcr_resultset_refcount(rs, x) == 1;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
new_2_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new_2() throws PCR_EXCEPTION_STRING if passed a null"
            " pointer for @name";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_resultset_new_2(NULL, SAMPLE_KEYS, SAMPLE_TYPES, SAMPLE_LEN,
                                   x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
new_2_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new_2() throws PCR_EXCEPTION_STRING if passed an"
            " empty string for @name";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_resultset_new_2("", SAMPLE_KEYS, SAMPLE_TYPES, SAMPLE_LEN,
                                   x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
new_2_test_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new_2() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @keys";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_resultset_new_2(SAMPLE_NAME, NULL, SAMPLE_TYPES, SAMPLE_LEN,
                                   x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
new_2_test_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new_2() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @keys";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_resultset_new_2(SAMPLE_NAME, SAMPLE_KEYS, NULL, SAMPLE_LEN,
                                   x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
new_2_test_7(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new_2() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @keys";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_resultset_new_2(SAMPLE_NAME, SAMPLE_KEYS, NULL, SAMPLE_LEN,
                                   x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
new_2_test_8(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new_2() throws PCR_EXCEPTION_RANGE if passed zero"
            " @len";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_resultset_new_2(SAMPLE_NAME, SAMPLE_KEYS, SAMPLE_TYPES, 0,
                                   x);
    }

    pcr_exception_catch (PCR_EXCEPTION_RANGE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_resultset_copy() test cases
 */


static bool
copy_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_copy() makes a copy of an existing resultset";

    pcr_exception_try (x) {
        pcr_resultset *rs = pcr_resultset_new_2(SAMPLE_NAME, SAMPLE_KEYS,
                                                SAMPLE_TYPES, SAMPLE_LEN, x);
        pcr_resultset *cp = pcr_resultset_copy(rs, x);

        return !pcr_resultset_rows(cp, x)
               && pcr_resultset_cols(cp, x) == SAMPLE_LEN
               && sample_types_match(pcr_resultset_types(cp, x), x)
               && sample_keys_match(pcr_resultset_keys(cp, x), x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
copy_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_copy() updates the reference count";

    pcr_exception_try (x) {
        pcr_resultset *rs = pcr_resultset_new_2(SAMPLE_NAME, SAMPLE_KEYS,
                                                SAMPLE_TYPES, SAMPLE_LEN, x);
        pcr_resultset *cp1 = pcr_resultset_copy(rs, x);
        pcr_resultset *cp2 = pcr_resultset_copy(cp1, x);

        (void) cp2;
        return pcr_resultset_refcount(rs, x) == 3;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
copy_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_copy() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @ctx";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_resultset_copy(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_resultset_push() test cases
 */


static bool
push_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_push() pushes attributes to a resultset";

    pcr_exception_try (x) {
        pcr_resultset *rs = pcr_resultset_new_2(SAMPLE_NAME, SAMPLE_KEYS,
                                                SAMPLE_TYPES, SAMPLE_LEN, x);
        sample_row_push(&rs, x);

        return sample_row_match(rs, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
push_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_push() respects reference counts";

    pcr_exception_try (x) {
        pcr_resultset *rs = pcr_resultset_new_2(SAMPLE_NAME, SAMPLE_KEYS,
                                                SAMPLE_TYPES, SAMPLE_LEN, x);
        pcr_resultset *cp1 = pcr_resultset_copy(rs, x);
        pcr_resultset *cp2 = pcr_resultset_copy(cp1, x);

        sample_row_push(&cp2, x);
        return pcr_resultset_refcount(cp2, x) == 1;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
push_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_push() throws PCR_EXCEPTION_HANDLE if passed a null"
            " handle for @ctx";

    pcr_exception_try (x) {
        pcr_log_suppress();
        sample_row_push(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
push_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_push() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @ctx";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_resultset *rs = NULL;
        sample_row_push(&rs, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_resultset_testsuite() interface
 */


static pcr_unittest *unit_tests[] = {
    &new_2_test_1, &new_2_test_2, &new_2_test_3, &new_2_test_4, &new_2_test_5,
    &new_2_test_6, &new_2_test_7, &new_2_test_8, &copy_test_1, &copy_test_2,
    &copy_test_3, &push_test_1, &push_test_2, &push_test_3, &push_test_4
};


extern pcr_testsuite *
pcr_resultset_testsuite(pcr_exception ex)
{
    pcr_exception_try (x) {
        const pcr_string *name = "PCR Resultset (pcr_resultset)";
        const size_t len = sizeof unit_tests / sizeof *unit_tests;

        return pcr_testsuite_new_2(name, unit_tests, len, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}

