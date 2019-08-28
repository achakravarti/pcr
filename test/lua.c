#include "suites.h"


#define SCRIPT_PATH "test/test.lua"


/******************************************************************************
 * pcr_lua_open() test cases
 */


static bool
open_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_open() opens an existing Lua script";

    pcr_exception_try (x) {
        pcr_lua *test = pcr_lua_open(SCRIPT_PATH, x);
        return pcr_lua_bool(test, "my_bool_var", x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
open_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_open() throws PCR_EXCEPTION_STRING if a null pointer is"
            " passed for @path";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_lua_open(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
open_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_open() throws PCR_EXCEPTION_STRING if a null string is"
            " passed for @path";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_lua_open("", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
open_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_open() throws PCR_EXCEPTION_FILE if @path does not exist";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_lua_open("non-existent.lua", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_FILE) {
        pcr_log_allow();
        return true;
    }

    return false;
    pcr_exception_unwind(ex);
}


/******************************************************************************
 * pcr_lua_bool() test cases
 */


static bool
bool_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_bool() reads a Boolean variable from a Lua script";

    pcr_exception_try (x) {
        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        return pcr_lua_bool(lua, "my_bool_var", x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
bool_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_bool() throws PCR_EXCEPTION_PARSE if @var does not exist"
            " in a Lua script";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_bool(lua, "non_existent_var", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_PARSE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
bool_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_bool() throws PCR_EXCEPTION_PARSE if @var is not a Boolean"
            " variable in a Lua script";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_bool(lua, "my_int_var", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_PARSE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
bool_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_bool() throws PCR_EXCEPTION_HANDLE if @ctx is null";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_lua_bool(NULL, "my_bool_var", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
bool_test_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_bool() throws PCR_EXCEPTION_STRING if @var is null";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_bool(lua, NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
bool_test_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_bool() throws PCR_EXCEPTION_STRING if @var is a null"
            " string";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_bool(lua, "", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_lua_int() test cases
 */


static bool
int_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_int() reads an integer variable from a Lua script";

    pcr_exception_try (x) {
        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        return pcr_lua_int(lua, "my_int_var", x) == -1024;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
int_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_int() throws PCR_EXCEPTION_PARSE if @var does not exist in"
            " a Lua script";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_int(lua, "non_existent_var", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_PARSE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
int_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_int() throws PCR_EXCEPTION_PARSE if @var is not an integer"
            "  variable in a Lua script";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_int(lua, "my_bool_var", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_PARSE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
int_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_int() throws PCR_EXCEPTION_HANDLE if @ctx is null";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_lua_int(NULL, "my_int_var", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
int_test_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_int() throws PCR_EXCEPTION_HANDLE if @var is null";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_int(lua, NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
int_test_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_int() throws PCR_EXCEPTION_HANDLE if @var is a null"
            " string";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_bool(lua, "", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_lua_float() test cases
 */


static bool
float_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_float() reads a floating point variable from a Lua script";

    pcr_exception_try (x) {
        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        double val = pcr_lua_float(lua, "my_float_var", x);

        return !pcr_string_cmp(pcr_string_float(val, x),
                               pcr_string_float(123.456789, x), x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
float_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_float() throws PCR_EXCEPTION_PARSE if @var does not exist"
            " in a Lua script";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_float(lua, "non_existent_var", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_PARSE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
float_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_float() throws PCR_EXCEPTION_PARSE if @var is not a"
            "  floating point variable in a Lua script";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_float(lua, "my_bool_var", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_PARSE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
float_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_float() throws PCR_EXCEPTION_HANDLE if @ctx is null";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_lua_float(NULL, "my_float_var", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
float_test_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_float() throws PCR_EXCEPTION_HANDLE if @var is null";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_float(lua, NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
float_test_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_float() throws PCR_EXCEPTION_HANDLE if @var is a null"
            " string";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_float(lua, "", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_lua_string() test cases
 */


static bool
string_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_string() reads an integer variable from a Lua script";

    pcr_exception_try (x) {
        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        pcr_string *val = pcr_lua_string(lua, "my_string_var", x);

        return !pcr_string_cmp(val, "Hello, мир!", x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
string_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_string() throws PCR_EXCEPTION_PARSE if @var does not exist"
            " in a Lua script";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_string(lua, "non_existent_var", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_PARSE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
string_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_string() throws PCR_EXCEPTION_PARSE if @var is not a"
            "  string variable in a Lua script";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_string(lua, "my_int_var", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_PARSE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
string_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_string() throws PCR_EXCEPTION_HANDLE if @ctx is null";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_lua_string(NULL, "my_string_var", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
string_test_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_string() throws PCR_EXCEPTION_HANDLE if @var is null";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_string(lua, NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
string_test_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_lua_string() throws PCR_EXCEPTION_HANDLE if @var is a null"
            " string";

    pcr_exception_try (x) {
        pcr_log_suppress();

        pcr_lua *lua = pcr_lua_open(SCRIPT_PATH, x);
        (void) pcr_lua_string(lua, "", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_lua_testsuite() interface
 */


static pcr_unittest *unit_tests[] = {
    &open_test_1,   &open_test_2,   &open_test_3,   &open_test_4,
    &bool_test_1,   &bool_test_2,   &bool_test_3,   &bool_test_4,
    &bool_test_5,   &bool_test_6,   &int_test_1,    &int_test_2,
    &int_test_3,    &int_test_4,    &int_test_5,    &int_test_6,
    &float_test_1,  &float_test_2,  &float_test_3,  &float_test_4,
    &float_test_5,  &float_test_6,  &string_test_1, &string_test_2,
    &string_test_3, &string_test_4, &string_test_5, &string_test_6
};


extern pcr_testsuite *
pcr_lua_testsuite(pcr_exception ex)
{
    pcr_exception_try (x) {
        const pcr_string *name = "PCR Lua (pcr_lua)";
        const size_t len = sizeof unit_tests / sizeof *unit_tests;

        return pcr_testsuite_new_2(name, unit_tests, len, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}

