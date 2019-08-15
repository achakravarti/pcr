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
 * pcr_sql_testsuite() interface
 */


static pcr_unittest *unit_tests[] = {
    &test_new_1, &test_new_2, &test_new_3, &test_new_4
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

