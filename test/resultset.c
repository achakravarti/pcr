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


/******************************************************************************
 * pcr_resultset_new() test cases
 */


static bool
test_new_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_resultset_new() creates a new result set";

    pcr_exception_try (x) {
        pcr_resultset *rs = pcr_resultset_new_2(SAMPLE_NAME, SAMPLE_KEYS,
                                                SAMPLE_TYPES, SAMPLE_LEN, x);

        return !pcr_resultset_rows(rs, x)
               && pcr_resultset_cols(rs, x) == SAMPLE_LEN;
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
        (void) pcr_resultset_new_2(NULL, SAMPLE_KEYS, SAMPLE_TYPES, SAMPLE_LEN,
                                   x);
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
        (void) pcr_resultset_new_2("", SAMPLE_KEYS, SAMPLE_TYPES, SAMPLE_LEN,
                                   x);
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
        (void) pcr_resultset_new_2(SAMPLE_NAME, NULL, SAMPLE_TYPES, SAMPLE_LEN,
                                   x);
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
        (void) pcr_resultset_new_2(SAMPLE_NAME, SAMPLE_KEYS, NULL, SAMPLE_LEN,
                                   x);
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
        const size_t len = sizeof unit_tests / sizeof *unit_tests;

        return pcr_testsuite_new_2(name, unit_tests, len, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}

