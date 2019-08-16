#include "./suites.h"


static pcr_string_vector *
mock_keys(pcr_exception ex)
{
    pcr_exception_try (x) {
        const pcr_string *keys[] = {"id", "fname", "lname", "attempts", "time"};
        return pcr_string_vector_new_2(keys, sizeof keys / sizeof *keys, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


static PCR_ATTRIBUTE_VECTOR *
mock_types(pcr_exception ex)
{
    pcr_exception_try (x) {
        PCR_ATTRIBUTE_VECTOR *types = pcr_vector_new(sizeof (PCR_ATTRIBUTE), x);

        PCR_ATTRIBUTE type = PCR_ATTRIBUTE_INT;
        pcr_vector_push(&types, &type, x);

        type = PCR_ATTRIBUTE_TEXT;
        pcr_vector_push(&types, &type, x);
        pcr_vector_push(&types, &type, x);

        type = PCR_ATTRIBUTE_INT;
        pcr_vector_push(&types, &type, x);

        type = PCR_ATTRIBUTE_FLOAT;
        pcr_vector_push(&types, &type, x);


        return types;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


static pcr_resultset *
mock_resultset(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string_vector *keys = mock_keys(x);
        PCR_ATTRIBUTE_VECTOR *types = mock_types(x);
        const pcr_string *name = "Test Resultset";

        return pcr_resultset_new(name, keys, types, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


/******************************************************************************
 * pcr_resultset_new() test cases
 */


static bool
test_new_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new() creates a new result set";

    pcr_exception_try (x) {
        pcr_resultset *rs = mock_resultset(x);
        return !pcr_resultset_rows(rs, x) && pcr_resultset_cols(rs, x) == 5;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new() throws PCR_EXCEPTION_STRING if passed a null"
            " pointer for @name";

    pcr_exception_try (x) {
        (void) pcr_resultset_new(NULL, mock_keys(x), mock_types(x), x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new() throws PCR_EXCEPTION_STRING if passed an"
            " empty string for @name";

    pcr_exception_try (x) {
        (void) pcr_resultset_new("", mock_keys(x), mock_types(x), x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @keys";

    pcr_exception_try (x) {
        (void) pcr_resultset_new("Test", NULL, mock_types(x), x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @keys";

    pcr_exception_try (x) {
        (void) pcr_resultset_new("Test", mock_keys(x), NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_resultset_testsuite() interface
 */


static pcr_unittest *unit_tests[] = {
    &test_new_1, &test_new_2, &test_new_3, &test_new_4, &test_new_5
};


extern pcr_testsuite *
pcr_resultset_testsuite(pcr_exception ex)
{
    pcr_exception_try (x) {
        const pcr_string *name = "PCR Resultset (pcr_resultset)";
        pcr_testsuite *ts = pcr_testsuite_new(name, x);

        register size_t len = sizeof unit_tests / sizeof *unit_tests;
        for (register size_t i = 0; i < len; i++)
            pcr_testsuite_push(ts, pcr_testcase_new(unit_tests[i], x), x);

        return ts;
    }

    pcr_exception_unwind(ex);
    return NULL;
}

