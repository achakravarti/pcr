#include "./api.h"


/** @private */
/* Define the pcr_sql struct; this structure was forward-declared in the API
 * header file as an abstract data type. */
struct pcr_sql {
    pcr_string *unbound; /* unbound SQL statement */
    pcr_string *bound;   /* bound SQL statement   */
    size_t ref;          /* reference count       */
};


/* Define the sql_fork() helper function. This function is responsible for
 * making a hard copy of @ctx if its reference count is greater than 1. In such
 * a case, the handle to the hard copy is returned. If the reference count of
 * @ctx is 1, then no hard copy is created, and a handle to it is returned. */
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


/* Implement the pcr_sql_new() interface function. We use the Boehm GC to
 * allocate memory for the new instance (through pcr_mempool_alloc()) and
 * initialise its fields as appropriate. */
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


/* Implement the pcr_sql_copy() interface function. Since pcr_sql instances are
 * reference counted, we simply need to return a handle to @ctx after updating
 * its reference count. */
extern pcr_sql *
pcr_sql_copy(const pcr_sql *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_sql *hnd = (pcr_sql *) ctx;
    hnd->ref++;

    return hnd;
}


/* Implement the pcr_sql_unbound() interface function. All we need to do is to
 * return a copy of the unbound field in @ctx. */
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


/* Implement the pcr_sql_bound() interface function. We return a copy of the
 * bound field in @ctx, taking care to check that it has been first set by a
 * prior call to pcr_sql_bind(). */
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


/* Implement the pcr_sql_refcount() interface function. We simply need to return
 * the ref field in @ctx. */
extern size_t
pcr_sql_refcount(const pcr_sql *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return ctx->ref;
}


/* Implement the pcr_sql_bind() interface function. We need to scan through the
 * unbound SQL statement (or the partially bound SQL statement), replacing each
 * instance of the parameter specified by the key of @attr with the
 * corresponding value. For text attributes, we need to take special care to
 * sanitise the value in order to prevent SQL injection attack vectors. This is
 * achieved in two steps:
 *   1. escaping all single quotes with double single quotes as prescribed by
 *      the SQL standard, and
 *   2. wrapping the entire text in single quotes to ensure that SQL views it as
 *      a string. */
extern void
pcr_sql_bind(pcr_sql **ctx, const pcr_attribute *attr, pcr_exception ex)
{
    pcr_sql *hnd;
    pcr_assert_handle(ctx && (hnd = *ctx), ex);

    pcr_exception_try (x) {
        pcr_string *param = pcr_attribute_key(attr, x);
        pcr_assert_state(pcr_string_find(hnd->unbound, param, x), x);

        pcr_string *arg = pcr_attribute_string(attr, x);

        if (pcr_attribute_type(attr, x) == PCR_ATTRIBUTE_TEXT) {
            pcr_string *sane = pcr_string_replace(arg, "\'", "\'\'", x);

            arg = pcr_string_new("\'", x);
            arg = pcr_string_add(arg, sane, x);
            arg = pcr_string_add(arg, "\'", x);
        }

        hnd = sql_fork(ctx, x);
        hnd->bound = pcr_string_replace(*hnd->bound ? hnd->bound
                                                    : hnd->unbound,
                                        param, arg, x);
    }

    pcr_exception_unwind(ex);
}


/* Implement the pcr_sql_reset() interface function. We reset the bound field of
 * @ctx after having taken care to make a hard copy of it (if required). */
extern void
pcr_sql_reset(pcr_sql **ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx && *ctx, ex);

    pcr_exception_try (x) {
        pcr_sql *hnd = sql_fork(ctx, x);
        hnd->bound = pcr_string_new("", x);
    }

    pcr_exception_unwind(ex);
}

