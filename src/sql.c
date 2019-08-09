#include "./api.h"


struct pcr_sql {
    pcr_string *unbound;
    pcr_string *bound;
    size_t refcount;
};

extern pcr_sql *
pcr_sql_new(const pcr_string *unbound, pcr_exception ex)
{
    pcr_assert_string(unbound, ex);

    pcr_exception_try (x) {
        pcr_sql *ctx = pcr_mempool_alloc(sizeof *ctx, x);

        ctx->refcount = 1;
        ctx->unbound = pcr_string_copy(unbound, x);
        ctx->bound = pcr_string_new("", x);

        return ctx;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_sql *
pcr_sql_copy(const pcr_sql *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_sql *hnd = (pcr_sql *) ctx;
    hnd->refcount++;

    return hnd;
}


extern pcr_string *
pcr_sql_unbound(const pcr_sql *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        return pcr_string_copy(ctx->unbound, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_string *
pcr_sql_bound(const pcr_sql *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        return pcr_string_copy(ctx->bound, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern void
pcr_sql_bind(pcr_sql *ctx, const pcr_field *field, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        pcr_string *param = pcr_string_new("@", x);
        param = pcr_string_add(param, pcr_field_key(field, x), x);
        pcr_assert_state(pcr_string_find(ctx->unbound, param, x), x);

        pcr_string *arg = pcr_field_string(field, x);
        if (pcr_field_type(field, x) == PCR_FIELD_TEXT) {
            pcr_string *sane = pcr_string_replaceall(arg, "\'", "\'\'", x);

            arg = pcr_string_new("\'", x);
            arg = pcr_string_add(arg, sane, x);
            arg = pcr_string_add(arg, "\'", x);
        }

        ctx->bound = pcr_string_replaceall(ctx->bound, param, arg, x);
    }

    pcr_exception_unwind(ex);
}

