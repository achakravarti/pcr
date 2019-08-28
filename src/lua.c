#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "api.h"


/** @private */
/* Define the pcr_lua struct; this structure was forward-declared in the API
 * header file as an abstract data type. I'm considering replacing this struct
 * with a void pointer as there is only one field in this struct. */
struct pcr_lua {
    lua_State *lua;
};


/* https://www.lua.org/pil/24.2.2.html */
#define INDEX_TOP -1
#define INDEX_FIRST 1


/* Define the var_parse() helper function. */
static void
var_parse(const pcr_lua *ctx, const pcr_string *var, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    pcr_assert_string(var, ex);

    const size_t len = 1024;
    pcr_string bfr[len];
    snprintf(bfr, len, "luaVar=%s", var);

    pcr_assert_parse(!luaL_dostring(ctx->lua, bfr), ex);
    lua_getglobal(ctx->lua, "luaVar");
}


/* Define the error_parse() helper function. */
static inline void
error_parse(const pcr_lua *ctx, const pcr_string *var)
{
    pcr_log_error("Failed to parse Lua var: %s", var);
    pcr_log_error("Lua says: %s", lua_tostring(ctx->lua, INDEX_TOP));
}


/* Implement the pcr_lua_open() interface function. */
extern pcr_lua *
pcr_lua_open(const pcr_string *path, pcr_exception ex)
{
    pcr_assert_string(path, ex);

    pcr_exception_try (x) {
        pcr_lua *hnd = pcr_mempool_alloc(sizeof *hnd, x);
        hnd->lua = luaL_newstate();

        luaL_openlibs(hnd->lua);
        pcr_assert_file(!luaL_dofile(hnd->lua, path), x);

        return hnd;
    }

    pcr_exception_catch (PCR_EXCEPTION_FILE) {
        pcr_log_error("Failed to load Lua file: %s", path);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


/* Implement the pcr_lua_close() interface function. */
extern void
pcr_lua_close(pcr_lua *ctx)
{
    if (pcr_hint_likely (ctx))
        lua_close(ctx->lua);
}


/* Implement the pcr_lua_bool() interface function. */
extern bool
pcr_lua_bool(const pcr_lua *ctx, const pcr_string *var, pcr_exception ex)
{
    pcr_exception_try (x) {
        var_parse(ctx, var, x);
        pcr_assert_parse(lua_isboolean(ctx->lua, INDEX_TOP), x);

        bool val = lua_toboolean(ctx->lua, INDEX_TOP);
        lua_pop(ctx->lua, INDEX_FIRST);

        return val;
    }

    pcr_exception_catch (PCR_EXCEPTION_PARSE) {
        error_parse(ctx, var);
    }

    pcr_exception_unwind(ex);
    return false;
}


/* Implement the pcr_lua_int() interface function. */
extern int64_t
pcr_lua_int(const pcr_lua *ctx, const pcr_string *var, pcr_exception ex)
{
    pcr_exception_try (x) {
        var_parse(ctx, var, x);
        pcr_assert_parse(lua_type(ctx->lua, INDEX_TOP) == LUA_TNUMBER, x);

        int64_t val = (int64_t) lua_tonumber(ctx->lua, INDEX_TOP);
        lua_pop(ctx->lua, INDEX_FIRST);

        return val;
    }

    pcr_exception_catch(PCR_EXCEPTION_PARSE) {
        error_parse(ctx, var);
    }

    pcr_exception_unwind(ex);
    return 0;
}


/* Implement the pcr_lua_float() interface function. */
extern double
pcr_lua_float(const pcr_lua *ctx, const pcr_string *var, pcr_exception ex)
{
    pcr_exception_try (x) {
        var_parse(ctx, var, x);
        pcr_assert_parse(lua_type(ctx->lua, INDEX_TOP) == LUA_TNUMBER, x);

        double val = lua_tonumber(ctx->lua, INDEX_TOP);
        lua_pop(ctx->lua, INDEX_FIRST);

        return val;
    }

    pcr_exception_catch(PCR_EXCEPTION_PARSE) {
        error_parse(ctx, var);
    }

    pcr_exception_unwind(ex);
    return 0.0;
}


/* Implement the pcr_lua_string() interface function. */
extern pcr_string *
pcr_lua_string(const pcr_lua *ctx, const pcr_string *var, pcr_exception ex)
{
    pcr_exception_try (x) {
        var_parse(ctx, var, x);
        pcr_assert_parse(lua_type(ctx->lua, INDEX_TOP) == LUA_TSTRING, x);

        pcr_string *val = pcr_string_copy(lua_tostring(ctx->lua, INDEX_TOP), x);
        lua_pop(ctx->lua, INDEX_FIRST);

        return val;
    }

    pcr_exception_catch(PCR_EXCEPTION_PARSE) {
        error_parse(ctx, var);
    }

    pcr_exception_unwind(ex);
    return "";
}

