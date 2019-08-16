#include "./suites.h"


/******************************************************************************
 * pcr_sql_new() test cases
 */


static bool
test_new_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_new() creates an unbound parameterised SQL statement";

    pcr_exception_try (x) {
        const pcr_string *psql = "SELECT * FROM users WHERE fname = @fname";
        pcr_sql *test = pcr_sql_new(psql, x);

        return !pcr_string_cmp(pcr_sql_unbound(test, x), psql, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_new_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_new() creates an SQL object with a refcount of 1";

    pcr_exception_try (x) {
        const pcr_string *psql = "SELECT * FROM users WHERE fname = @fname";
        pcr_sql *test = pcr_sql_new(psql, x);

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
        const pcr_string *psql = "SELECT * FROM users WHERE fname = @fname";
        pcr_sql *test = pcr_sql_new(psql, x);
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
        const pcr_string *psql = "SELECT * FROM users WHERE fname = @fname";
        pcr_sql *test = pcr_sql_new(psql, x);

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
        const pcr_string *psql = "SELECT * FROM users where fname = @fname";
        pcr_sql *test = pcr_sql_new(psql, x);
        (void) pcr_sql_bound(test, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STATE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_sql_bind() test cases
 */


static bool
test_bind_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bind() binds a null parameter";

    pcr_exception_try (x) {
        const pcr_string *psql = "SELECT * FROM users WHERE fname = @fname";
        pcr_sql *test = pcr_sql_new(psql, x);
        pcr_sql_bind_null(&test, "@fname", x);

        const pcr_string *sql = "SELECT * FROM users WHERE fname = NULL";
        return !pcr_string_cmp(sql, pcr_sql_bound(test, x), x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_bind_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bind() binds an int parameter";

    pcr_exception_try (x) {
        const pcr_string *psql = "SELECT * FROM users WHERE age > @age";
        pcr_sql *test = pcr_sql_new(psql, x);
        pcr_sql_bind_int(&test, "@age", 43, x);

        const pcr_string *sql = "SELECT * FROM users WHERE age > 43";
        return !pcr_string_cmp(sql, pcr_sql_bound(test, x), x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_bind_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bind() binds a float parameter";

    pcr_exception_try (x) {
        const pcr_string *psql = "SELECT * FROM users WHERE time <= @time";
        pcr_sql *test = pcr_sql_new(psql, x);
        pcr_sql_bind_float(&test, "@time", 123.456789, x);

        const pcr_string *sql = "SELECT * FROM users WHERE time <= 123.456789";
        return !pcr_string_cmp(sql, pcr_sql_bound(test, x), x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_bind_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bind() binds an ASCII text parameter";

    pcr_exception_try (x) {
        const pcr_string *psql = "SELECT id FROM msgs WHERE msg = @msg";
        pcr_sql *test = pcr_sql_new(psql, x);
        pcr_sql_bind_text(&test, "@msg", "Hello, world!", x);

        const pcr_string *sql = "SELECT id FROM msgs WHERE msg = \'Hello, world"
                                "!\'";
        return !pcr_string_cmp(sql, pcr_sql_bound(test, x), x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_bind_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bind() binds a Unicode text parameter";

    pcr_exception_try (x) {
        const pcr_string *psql = "SELECT id FROM msgs WHERE msg = @msg";
        pcr_sql *test = pcr_sql_new(psql, x);
        pcr_sql_bind_text(&test, "@msg", "Привет, мир!", x);

        const pcr_string *sql = "SELECT id FROM msgs WHERE msg = \'Привет, мир!"
                                "\'";
        return !pcr_string_cmp(sql, pcr_sql_bound(test, x), x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_bind_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bind() binds multiple parameters of different types";

    pcr_exception_try (x) {
        const pcr_string *psql = "SELECT * FROM foo WHERE user <> @user AND"
                                 " attempts > @attempts AND time <= @time AND"
                                 " en = @en AND ru = @ru";
        pcr_sql *test = pcr_sql_new(psql, x);

        pcr_sql_bind_null(&test, "@user", x);
        pcr_sql_bind_int(&test, "@attempts", 1024, x);
        pcr_sql_bind_float(&test, "@time", 123.456789, x);
        pcr_sql_bind_text(&test, "@en",  "Hello, world!", x);
        pcr_sql_bind_text(&test, "@ru", "Привет, мир!", x);

        pcr_log_trace("bound = %s", pcr_sql_bound(test, x));
        const pcr_string *sql = "SELECT * FROM foo WHERE user <> NULL AND"
                                " attempts > 1024 AND time <= 123.456789 AND"
                                " en = \'Hello, world!\' AND"
                                " ru = \'Привет, мир!\'";
        return !pcr_string_cmp(sql, pcr_sql_bound(test, x), x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_bind_7(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bind() throws PCR_EXCEPTION_HANDLE if passed a null"
            " handle for @ctx";

    pcr_exception_try (x) {
        pcr_sql_bind_null(NULL, "key", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_bind_8(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bind() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @ctx";

    pcr_exception_try (x) {
        pcr_sql *test = NULL;
        pcr_sql_bind_null(&test, "key", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_bind_9(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bind() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @attr";

    pcr_exception_try (x) {
        const pcr_string *psql = "SELECT * FROM users WHERE fname = @fname";
        pcr_sql *test = pcr_sql_new(psql, x);
        (void) pcr_sql_bind(&test, NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_bind_10(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bind() throws PCR_EXCEPTION_STATE if passed a parameter"
            " that does not exist";

    pcr_exception_try (x) {
        const pcr_string *psql = "SELECT * FROM users WHERE fname = @fname";
        pcr_sql *test = pcr_sql_new(psql, x);
        (void) pcr_sql_bind_null(&test, "@lname", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STATE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_bind_11(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bind() respects reference counts";

    pcr_exception_try (x) {
        const pcr_string *psql = "SELECT * FROM users WHERE fname = @fname";
        pcr_sql *test = pcr_sql_new(psql, x);
        pcr_sql *copy = pcr_sql_copy(test, x);

        const size_t refc = pcr_sql_refcount(test, x);
        pcr_sql_bind_null(&test, "@fname", x);

        return refc == 2 && pcr_sql_refcount(test, x) == 1
               && pcr_sql_refcount(copy, x) == 1;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_sql_reset() test cases
 */

static bool
test_reset_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_reset() resets the bound SQL statement";

    pcr_exception_try (x) {
        const pcr_string *psql = "SELECT * FROM users WHERE fname = @fname";
        pcr_sql *test = pcr_sql_new(psql, x);
        pcr_sql_bind_text(&test, "@fname", "foo", x);

        pcr_sql_reset(&test, x);
        (void) pcr_sql_bound(test, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STATE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_reset_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_reset() throws PCR_EXCEPTION_HANDLE if passed a null"
            " pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_sql_reset(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
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
    &test_bound_1, &test_bound_2, &test_bind_1, &test_bind_2, &test_bind_3,
    &test_bind_4, &test_bind_5, &test_bind_6, &test_bind_7, &test_bind_8,
    &test_bind_9, &test_bind_10, &test_bind_11, &test_reset_1, &test_reset_2
};


extern pcr_testsuite *
pcr_sql_testsuite(pcr_exception ex)
{
    pcr_exception_try (x) {
        const pcr_string *name = "PCR SQL (pcr_sql)";
        const size_t len = sizeof unit_tests / sizeof *unit_tests;

        return pcr_testsuite_new_2(name, unit_tests, len, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}

