#include "./api.h"


struct pcr_sql {
    pcr_string *unbound;
    pcr_string *bound;
    size_t ref;
};

extern pcr_sql *
pcr_sql_new(const pcr_string *unbound, pcr_exception ex)
{
    pcr_assert_string(unbound, ex);

    pcr_exception_try (x) {
        pcr_sql *ctx = pcr_mempool_alloc(sizeof *ctx, x);

        ctx->ref = 1;
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
    hnd->ref++;

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
    pcr_assert_state(*ctx->bound, ex);

    pcr_exception_try (x) {
        return pcr_string_copy(ctx->bound, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern size_t
pcr_sql_refcount(const pcr_sql *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return ctx->ref;
}


static pcr_sql *
sql_fork(pcr_sql **ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_sql *hnd = *ctx;

        if (hnd->ref > 1) {
            hnd->ref--;
            pcr_string *unbound = pcr_string_copy(hnd->unbound, x);
            pcr_string *bound = pcr_string_copy(hnd->bound, x);

            hnd = *ctx = pcr_mempool_alloc(sizeof *hnd, x);
            hnd->unbound = unbound;
            hnd->bound = bound;
            hnd->ref = 1;
        }

        return hnd;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern void
pcr_sql_bind(pcr_sql **ctx, const pcr_attribute *attr, pcr_exception ex)
{
    pcr_sql *hnd;
    pcr_assert_handle(ctx && (hnd = *ctx), ex);

    pcr_exception_try (x) {
        pcr_string *param = pcr_string_new("@", x);
        param = pcr_string_add(param, pcr_attribute_key(attr, x), x);
        pcr_assert_state(pcr_string_find(hnd->unbound, param, x), x);

        pcr_string *arg = pcr_attribute_string(attr, x);

        if (pcr_attribute_type(attr, x) == PCR_ATTRIBUTE_TEXT) {
            pcr_string *sane = pcr_string_replaceall(arg, "\'", "\'\'", x);

            arg = pcr_string_new("\'", x);
            arg = pcr_string_add(arg, sane, x);
            arg = pcr_string_add(arg, "\'", x);
        }

        hnd = sql_fork(ctx, x);
        hnd->bound = pcr_string_replaceall(hnd->unbound, param, arg, x);
    }

    pcr_exception_unwind(ex);
}


extern void
pcr_sql_reset(pcr_sql **ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx && *ctx, ex);

    pcr_exception_try (x) {
        pcr_sql *hnd = sql_fork(ctx, x);
        //hnd->bound = pcr_string_copy(hnd->unbound, x);
        hnd->bound = pcr_string_new("", x);
    }

    pcr_exception_unwind(ex);
}

