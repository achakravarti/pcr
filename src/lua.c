#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "api.h"


struct pcr_lua {
    lua_State *lua;
};


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


static inline void
error_parse(const pcr_lua *ctx, const pcr_string *var)
{
    pcr_log_error("Failed to parse Lua var: %s", var);
    pcr_log_error("Lua says: %s", lua_tostring(ctx->lua, -1));
}


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


extern void
pcr_lua_close(pcr_lua *ctx)
{
    if (pcr_hint_likely (ctx))
        lua_close(ctx->lua);
}


extern bool
pcr_lua_bool(const pcr_lua *ctx, const pcr_string *var, pcr_exception ex)
{
    pcr_exception_try (x) {
        var_parse(ctx, var, x);
        pcr_assert_parse(lua_isboolean(ctx->lua, -1), x);

        bool val = lua_toboolean(ctx->lua, -1);
        lua_pop(ctx->lua, 1);

        return val;
    }

    pcr_exception_catch (PCR_EXCEPTION_PARSE) {
        error_parse(ctx, var);
    }

    pcr_exception_unwind(ex);
    return false;
}


extern int64_t
pcr_lua_int(const pcr_lua *ctx, const pcr_string *var, pcr_exception ex)
{
    pcr_exception_try (x) {
        var_parse(ctx, var, x);
        pcr_assert_parse(lua_isnumber(ctx->lua, -1), x);

        int64_t val = (int64_t) lua_tonumber(ctx->lua, -1);
        lua_pop(ctx->lua, 1);

        return val;
    }

    pcr_exception_catch(PCR_EXCEPTION_PARSE) {
        error_parse(ctx, var);
    }

    pcr_exception_unwind(ex);
    return 0;
}


extern double
pcr_lua_float(const pcr_lua *ctx, const pcr_string *var, pcr_exception ex)
{
    pcr_exception_try (x) {
        var_parse(ctx, var, x);
        pcr_assert_parse(lua_isnumber(ctx->lua, -1), x);

        double val = lua_tonumber(ctx->lua, -1);
        lua_pop(ctx->lua, 1);

        return val;
    }

    pcr_exception_catch(PCR_EXCEPTION_PARSE) {
        error_parse(ctx, var);
    }

    pcr_exception_unwind(ex);
    return 0.0;
}


extern pcr_string *
pcr_lua_string(const pcr_lua *ctx, const pcr_string *var, pcr_exception ex)
{
    pcr_exception_try (x) {
        var_parse(ctx, var, x);
        pcr_assert_parse(lua_isstring(ctx->lua, -1), x);

        pcr_string *val = pcr_string_copy(lua_tostring(ctx->lua, -1), x);
        lua_pop(ctx->lua, 1);

        return val;
    }

    pcr_exception_catch(PCR_EXCEPTION_PARSE) {
        error_parse(ctx, var);
    }

    pcr_exception_unwind(ex);
    return "";
}

