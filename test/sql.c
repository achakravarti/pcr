#include "./suites.h"


#define PSQL_SELECT "SELECT * FROM users WHERE fname = @fname"


/******************************************************************************
 * pcr_sql_new() test cases
 */

static bool
test_new_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_new() creates an unbound parameterised SQL statement";

    pcr_exception_try (x) {
        pcr_sql *test = pcr_sql_new(PSQL_SELECT, x);
        return !pcr_string_cmp(pcr_sql_unbound(test, x), PSQL_SELECT, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_new() creates an SQL object with a refcount of 1";

    pcr_exception_try (x) {
        pcr_sql *test = pcr_sql_new(PSQL_SELECT, x);
        return pcr_sql_refcount(test, x) == 1;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_new() throws PCR_EXCEPTION_STRING if passed a null pointer"
            " for @unbound";

    pcr_exception_try (x) {
        (void) pcr_sql_new(NULL, x);
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
    *desc = "pcr_sql_new() throws PCR_EXCEPTION_STRING if passed an empty"
            " string for @unbound";

    pcr_exception_try (x) {
        (void) pcr_sql_new("", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_sql_copy() test cases
 */


static bool
test_copy_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_copy() clones an existing SQL instance";

    pcr_exception_try (x) {
        pcr_sql *test = pcr_sql_new(PSQL_SELECT, x);
        pcr_sql *copy = pcr_sql_copy(test, x);

        return !pcr_string_cmp(pcr_sql_unbound(test, x),
                               pcr_sql_unbound(copy, x), x)
               && pcr_sql_refcount(test, x) == pcr_sql_refcount(copy, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_copy_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_copy() updates the reference count of an SQL instance";

    pcr_exception_try (x) {
        pcr_sql *test = pcr_sql_new(PSQL_SELECT, x);
        pcr_sql *cpy1 = pcr_sql_copy(test, x);
        pcr_sql *cpy2 = pcr_sql_copy(cpy1, x);

        (void) cpy2;
        return pcr_sql_refcount(test, x) == 3;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_copy_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_copy() throws PCR_EXCEPTION_HANDLE if passed a null"
            " for @ctx";

    pcr_exception_try (x) {
        (void) pcr_sql_copy(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_sql_refcount() test cases
 */


static bool
test_refcount_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_refcount() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_sql_refcount(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_sql_unbound() test cases
 */


static bool
test_unbound_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_unbound() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_sql_unbound(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_sql_bound() test cases
 */


static bool
test_bound_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bound() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_sql_bound(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_bound_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bound() throws PCR_EXCEPTION_STATE if called without bound"
            " parameters";

    pcr_exception_try (x) {
        pcr_sql *test = pcr_sql_new(PSQL_SELECT, x);
        (void) pcr_sql_bound(test, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STATE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_sql_testsuite() interface
 */


static pcr_unittest *unit_tests[] = {
    &test_new_1, &test_new_2, &test_new_3, &test_new_4, &test_copy_1,
    &test_copy_2, &test_copy_3, &test_refcount_1, &test_unbound_1,
    &test_bound_1, &test_bound_2
};


extern pcr_testsuite *
pcr_sql_testsuite(pcr_exception ex)
{
    pcr_exception_try (x) {
        const pcr_string *name = "PCR SQL (pcr_sql)";
        pcr_testsuite *ts = pcr_testsuite_new(name, x);

        register size_t len = sizeof unit_tests / sizeof *unit_tests;
        for (register size_t i = 0; i < len; i++)
            pcr_testsuite_push(ts, pcr_testcase_new(unit_tests[i], x), x);

        return ts;
    }

    pcr_exception_unwind(ex);
    return NULL;
}

