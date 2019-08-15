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

        pcr_attribute *attr = pcr_attribute_new(PCR_ATTRIBUTE_NULL, "fname",
                                                NULL, x);
        pcr_sql_bind(&test, attr, x);

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

        const int64_t val = 43;
        pcr_attribute *attr = pcr_attribute_new(PCR_ATTRIBUTE_INT, "age", &val,
                                                x);
        pcr_sql_bind(&test, attr, x);

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

        const double val = 123.456789;
        pcr_attribute *attr = pcr_attribute_new(PCR_ATTRIBUTE_FLOAT, "time",
                                                &val, x);
        pcr_sql_bind(&test, attr, x);

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

        const pcr_string *val = "Hello, world!";
        pcr_attribute *attr = pcr_attribute_new(PCR_ATTRIBUTE_TEXT, "msg", val,
                                                x);
        pcr_sql_bind(&test, attr, x);
        pcr_log_trace("bound = %s", pcr_sql_bound(test, x));

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
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
test_bind_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_sql_bind() binds multiple parameters of different types";

    pcr_exception_try (x) {
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
        pcr_attribute *attr = pcr_attribute_new(PCR_ATTRIBUTE_TEXT, "key",
                                                "val", x);
        pcr_sql_bind(NULL, attr, x);
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
        pcr_attribute *attr = pcr_attribute_new(PCR_ATTRIBUTE_TEXT, "key",
                                                "val", x);
        pcr_sql *test = NULL;
        pcr_sql_bind(&test, attr, x);
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


/******************************************************************************
 * pcr_sql_testsuite() interface
 */


static pcr_unittest *unit_tests[] = {
    &test_new_1, &test_new_2, &test_new_3, &test_new_4, &test_copy_1,
    &test_copy_2, &test_copy_3, &test_refcount_1, &test_unbound_1,
    &test_bound_1, &test_bound_2, &test_bind_1, &test_bind_2, &test_bind_3,
    &test_bind_4, &test_bind_5, &test_bind_6, &test_bind_7, &test_bind_8,
    &test_bind_9
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

