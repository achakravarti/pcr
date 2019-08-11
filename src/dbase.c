#include <sqlite3.h>
#include "./api.h"


/******************************************************************************
 * Data structures
 */


struct vtable {
    pcr_resultset *
    (*query)(void *adapter, const pcr_sql *sql, pcr_exception ex);

    void
    (*command)(void *adapter, const pcr_sql *sql, pcr_exception ex);

    void
    (*begin)(void *adapter, pcr_exception ex);

    void
    (*commit)(void *adapter, pcr_exception ex);

    void
    (*rollback)(void *adapter, pcr_exception ex);
};


struct pcr_dbase {
    struct vtable *vtable;
    void *adapter;
    pcr_string *conn;
    PCR_DBASE engine;
};


/******************************************************************************
 * Sqlite adapter helpers
 */


static sqlite3_stmt *
sqlite_stmt_init(void *adapter, const pcr_sql *sql, pcr_exception ex)
{
    sqlite3_stmt *stmt = NULL;

    pcr_exception_try (x) {
        pcr_string *bound = pcr_sql_bound(sql, ex);
        int len = (int) pcr_string_len(bound, ex) + 1;

        int rc = sqlite3_prepare_v2((sqlite3 *) adapter, bound, len, &stmt,
                                    NULL);
        pcr_assert_state(rc == SQLITE_OK, ex);
    }

    pcr_exception_unwind(ex);
    return stmt;
}

static inline const pcr_string *
sqlite_col_key(sqlite3_stmt *stmt, int col)
{
    const pcr_string *key = sqlite3_column_name(stmt, col);
    return key ? key : "";
}


static inline PCR_ATTRIBUTE
sqlite_col_type(sqlite3_stmt *stmt, int col)
{
    switch (sqlite3_column_type(stmt, col)) {
        case SQLITE_INTEGER: return PCR_ATTRIBUTE_INT; break;
        case SQLITE_FLOAT: return PCR_ATTRIBUTE_FLOAT; break;
        case SQLITE_TEXT: return PCR_ATTRIBUTE_TEXT; break;
        default: return PCR_ATTRIBUTE_NULL; break;
    }
}


static inline void *
sqlite_col_value(sqlite3_stmt *stmt, int col, pcr_exception ex)
{
    void *val = NULL;

    pcr_exception_try (x) {
        int type = sqlite3_column_type(stmt, col);

        if (type == SQLITE_INTEGER) {
            int64_t *ival = pcr_mempool_alloc(sizeof *ival, x);
            *ival = sqlite3_column_int64(stmt, col);
            val = ival;
        }

        else if (type == SQLITE_FLOAT) {
            double *fval = pcr_mempool_alloc(sizeof *fval, x);
            *fval = sqlite3_column_double(stmt, col);
            val = fval;
        }

        else if (type == SQLITE_TEXT)
            val = pcr_string_new((char *) sqlite3_column_text(stmt, col), x);
    }

    pcr_exception_unwind(ex);
    return val;
}


static pcr_resultset *
sqlite_rs_init(sqlite3_stmt *stmt, pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string_vector *keys = pcr_vector_new(sizeof (pcr_string), x);
        PCR_ATTRIBUTE_VECTOR *types = pcr_vector_new(sizeof (PCR_ATTRIBUTE), x);

        PCR_ATTRIBUTE type;
        register int cols = sqlite3_column_count(stmt);
        for (register int i = 0; i < cols; i++) {
            pcr_vector_push(&keys, sqlite_col_key(stmt, i), ex);
            type = sqlite_col_type(stmt, i);
            pcr_vector_push(&types, &type, ex);
        }

        return pcr_resultset_new("resultset", keys, types, ex);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


static void
sqlite_rs_fill(pcr_resultset *rs, sqlite3_stmt *stmt, pcr_exception ex)
{
    pcr_exception_try (x) {
        register int rc = sqlite3_step(stmt);
        register int cols = sqlite3_column_count(stmt);

        pcr_attribute *attr;
        while (rc != SQLITE_DONE) {
            for (register int i = 0; i < cols; i++) {
                attr = pcr_attribute_new(sqlite_col_type(stmt, i),
                                         sqlite_col_key(stmt, i),
                                         sqlite_col_value(stmt, i, x), x);
                pcr_resultset_push(&rs, attr, x);
            }

            rc = sqlite3_step(stmt);
        }
    }

    pcr_exception_unwind(ex);
}


static pcr_resultset *
sqlite_exec_query(sqlite3_stmt *stmt, pcr_exception ex)
{
    pcr_resultset *rs = NULL;

    pcr_exception_try (x) {
        rs = sqlite_rs_init(stmt, x);
        sqlite_rs_fill(rs, stmt, x);
    }

    pcr_exception_unwind(ex);
    return rs;
}


static inline void
sqlite_exec_cmd(void *adapter, const pcr_string *stmt, pcr_exception ex)
{
    int rc = sqlite3_exec((sqlite3 *) adapter, stmt, NULL, NULL, NULL);
    pcr_assert_state(rc == SQLITE_OK, ex);
}


/******************************************************************************
 * Sqlite adapter callbacks
 */


static pcr_resultset *
sqlite_query(void *adapter, const pcr_sql *sql, pcr_exception ex)
{
    sqlite3_stmt *stmt = NULL;
    pcr_resultset *rs;

    pcr_exception_try (x) {
        stmt = sqlite_stmt_init(adapter, sql, x);
        rs = sqlite_exec_query(stmt, x);
    }

    (void) sqlite3_finalize(stmt);

    pcr_exception_unwind(ex);
    return rs;
}


static void
sqlite_command(void *adapter, const pcr_sql *sql, pcr_exception ex)
{
    sqlite_exec_cmd(adapter, pcr_sql_bound(sql, ex), ex);
}


static inline void
sqlite_begin(void *adapter, pcr_exception ex)
{
    sqlite_exec_cmd(adapter, "BEGIN;", ex);
}


static inline void
sqlite_commit(void *adapter, pcr_exception ex)
{
    sqlite_exec_cmd(adapter, "COMMIT;", ex);
}


static inline void
sqlite_rollback(void *adapter, pcr_exception ex)
{
    sqlite_exec_cmd(adapter, "ROLLBACK;", ex);
}


static void
sqlite_init(pcr_dbase *ctx, pcr_exception ex)
{

    sqlite3 *adapter;
    pcr_assert_state(sqlite3_open(ctx->conn, &adapter) == SQLITE_OK, ex);
    ctx->adapter = adapter;

    ctx->vtable->begin = &sqlite_begin;
    ctx->vtable->command = &sqlite_command;
    ctx->vtable->commit = &sqlite_commit;
    ctx->vtable->query = &sqlite_query;
    ctx->vtable->rollback = &sqlite_rollback;
}


/******************************************************************************
 * pcr_dbase
 */


extern pcr_dbase *
pcr_dbase_new(PCR_DBASE engine, const pcr_string *conn, pcr_exception ex)
{
    pcr_assert_string(conn, ex);

    pcr_exception_try (x) {
        pcr_dbase *ctx = pcr_mempool_alloc(sizeof *ctx, x);

        ctx->engine = engine;
        ctx->conn = pcr_string_copy(conn, x);
        ctx->vtable = pcr_mempool_alloc(sizeof *ctx->vtable, x);

        switch (engine) {
            default:
                sqlite_init(ctx, x);
                break;
        }

        return ctx;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_dbase *
pcr_dbase_copy(const pcr_dbase *ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        return pcr_dbase_new(ctx->engine, ctx->conn, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_resultset *
pcr_dbase_query(const pcr_dbase *ctx, const pcr_sql *sql, pcr_exception ex)
{
    pcr_assert_handle(ctx && sql, ex);

    pcr_exception_try (x) {
        return ctx->vtable->query(ctx->adapter, sql, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern void
pcr_dbase_command(pcr_dbase *ctx, const pcr_sql *sql, pcr_exception ex)
{
    pcr_assert_handle(ctx && sql, ex);

    pcr_exception_try (x) {
        ctx->vtable->command(ctx->adapter, sql, x);
    }

    pcr_exception_unwind(ex);
}


extern void
pcr_dbase_begin(pcr_dbase *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        ctx->vtable->begin(ctx->adapter, x);
    }

    pcr_exception_unwind(ex);
}


extern void
pcr_dbase_commit(pcr_dbase *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        ctx->vtable->commit(ctx->adapter, x);
    }

    pcr_exception_unwind(ex);
}


extern void
pcr_dbase_rollback(pcr_dbase *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        ctx->vtable->rollback(ctx->adapter, x);
    }

    pcr_exception_unwind(ex);
}

