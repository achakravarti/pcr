#include <string.h>
#include "./api.h"


struct pcr_vector {
    void **payload;
    size_t sz;
    size_t len;
    size_t cap;
    size_t ref;
};


extern pcr_vector *pcr_vector_new(size_t elemsz, pcr_exception ex)
{
    pcr_assert_range(elemsz, ex);

    pcr_exception_try (x) {
        pcr_vector *ctx = pcr_mempool_alloc(sizeof *ctx, x);

        ctx->sz = elemsz;
        ctx->len = ctx->ref = 0;
        ctx->cap = 4;
        ctx->payload = pcr_mempool_alloc(elemsz * ctx->cap, x);

        return ctx;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_vector *pcr_vector_copy(const pcr_vector *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_vector *hnd = (pcr_vector *) ctx;
    hnd->ref++;

    return hnd;
}


extern size_t pcr_vector_len(const pcr_vector *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    return ctx->len;
}


extern size_t pcr_vector_refcount(const pcr_vector *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    return ctx->ref;
}


extern void *pcr_vector_elem(const pcr_vector *ctx, size_t idx,
                                    pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    pcr_assert_range(idx && idx <= ctx->len, ex);

    pcr_exception_try (x) {
        void *elem = pcr_mempool_alloc(ctx->sz, x);
        memcpy(elem, ctx->payload[idx - 1], ctx->sz);

        return elem;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


static pcr_vector *vec_fork(pcr_vector *ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_vector *frk = pcr_vector_new(ctx->sz, x);

        frk->sz = ctx->sz;
        frk->len = ctx->len;
        frk->cap = ctx->cap;
        frk->ref = --ctx->ref;

        const size_t newsz = frk->sz * frk->cap;
        frk->payload = pcr_mempool_alloc(newsz, x);
        memcpy(frk->payload, ctx->payload, newsz);

        return frk;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern void pcr_vector_setelem(pcr_vector **ctx, const void *elem, size_t idx,
                                    pcr_exception ex)
{
    pcr_assert_handle(ctx && *ctx && elem, ex);
    pcr_assert_range(idx && idx <= (*ctx)->len, ex);

    pcr_exception_try (x) {
        if ((*ctx)->ref > 1)
            *ctx = vec_fork(*ctx, x);

        memcpy((*ctx)->payload[idx - 1], elem, (*ctx)->sz);
    }

    pcr_exception_unwind(ex);
}


extern void pcr_vector_iterate(const pcr_vector *ctx, pcr_iterator *itr,
                                    void *opt, pcr_exception ex)
{
    pcr_assert_handle(ctx && itr, ex);

    pcr_exception_try (x) {
        register size_t len = ctx->len;
        for (register size_t i = 0; i < len; i++)
            itr(ctx->payload[i], i, opt, x);
    }

    pcr_exception_unwind(ex);
}


extern void pcr_vector_muterate(pcr_vector **ctx, pcr_muterator *mtr, void *opt,
                                        pcr_exception ex)
{
    pcr_assert_handle(ctx && *ctx && mtr, ex);

    pcr_exception_try (x) {
        if ((*ctx)->ref > 1)
            *ctx = vec_fork(*ctx, x);

        register size_t len = (*ctx)->len;
        for (register size_t i = 0; i < len; i++)
            mtr((*ctx)->payload[i], i, opt, x);
    }

    pcr_exception_unwind(ex);
}

