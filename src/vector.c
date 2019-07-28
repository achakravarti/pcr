#include <stdlib.h>
#include <string.h>
#include "./api.h"


struct pcr_vector {
    void **payload;
    size_t sz;
    size_t len;
    size_t cap;
    size_t ref;
    bool sorted;
};


extern pcr_vector *pcr_vector_new(size_t elemsz, pcr_exception ex)
{
    pcr_assert_range(elemsz, ex);

    pcr_exception_try (x) {
        pcr_vector *ctx = pcr_mempool_alloc(sizeof *ctx, x);

        ctx->sz = elemsz;
        ctx->len = ctx->ref = 0;
        ctx->cap = 4;
        ctx->sorted = false;
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


extern bool pcr_vector_sorted(const pcr_vector *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return ctx->sorted;
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


static pcr_vector *vec_fork(pcr_vector **ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_vector *hnd = *ctx;
        if (hnd->ref > 1) {
            pcr_vector *frk = pcr_vector_new(hnd->sz, x);

            frk->sz = hnd->sz;
            frk->len = hnd->len;
            frk->cap = hnd->cap;
            frk->ref = --hnd->ref;
            frk->sorted = hnd->sorted;

            const size_t newsz = frk->sz * frk->cap;
            frk->payload = pcr_mempool_alloc(newsz, x);
            memcpy(frk->payload, hnd->payload, newsz);

            *ctx = frk;
        }

        return *ctx;
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
        pcr_vector *hnd = vec_fork(ctx, x);
        memcpy(hnd->payload[idx - 1], elem, hnd->sz);
    }

    pcr_exception_unwind(ex);
}


extern void pcr_vector_push(pcr_vector **ctx, const void *elem,
                                    pcr_exception ex)
{
    pcr_assert_handle(ctx && *ctx && elem, ex);

    pcr_exception_try (x) {
        pcr_vector *hnd = vec_fork(ctx, x);
        if (pcr_hint_unlikely (hnd->len == hnd->cap)) {
            hnd->cap *= 2;
            hnd->payload = pcr_mempool_realloc(hnd->payload, hnd->sz * hnd->cap,
                                                    x);
        }

        hnd->payload[hnd->len] = pcr_mempool_realloc(hnd->payload[hnd->len],
                                                            hnd->sz, x);
        memcpy(hnd->payload[hnd->len++], elem, hnd->sz);
        hnd->sorted = false;
    }

    pcr_exception_unwind(ex);
}


extern void pcr_vector_pop(pcr_vector **ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx && *ctx, ex);

    pcr_exception_try (x) {
        pcr_vector *hnd = vec_fork(ctx, x);
        if (pcr_hint_likely (hnd->len)) {
            hnd->payload[hnd->len--] = NULL;
            hnd->sorted = false;
        }
    }

    pcr_exception_unwind(ex);
}


extern void pcr_vector_sort(pcr_vector **ctx, pcr_comparator *cmp,
                                    pcr_exception ex)
{
    pcr_assert_handle(ctx && *ctx && cmp, ex);

    pcr_exception_try (x) {
        pcr_vector *hnd = vec_fork(ctx, x);
        if (!hnd->sorted) {
            qsort(hnd->payload, hnd->len, hnd->sz, cmp);
            hnd->sorted = true;
        }
    }

    pcr_exception_unwind(ex);
}


extern size_t pcr_vector_search(pcr_vector **ctx, const void *key,
                                        pcr_comparator *cmp, pcr_exception ex)
{
    pcr_assert_handle(ctx && *ctx && key && cmp, ex);

    pcr_exception_try (x) {
        pcr_vector_sort(ctx, cmp, x);

        pcr_vector *hnd = *ctx;
        void *where = bsearch(key, hnd->payload, hnd->len, hnd->sz, cmp);
        return where ? ((void **) where - hnd->payload) / hnd->sz : 0;
    }

    pcr_exception_unwind(ex);
    return 0;
}


extern void pcr_vector_iterate(const pcr_vector *ctx, pcr_iterator *itr,
                                    void *opt, pcr_exception ex)
{
    pcr_assert_handle(ctx && itr, ex);

    pcr_exception_try (x) {
        for (register size_t i = 0, len = ctx->len; i < len; i++)
            itr(ctx->payload[i], i + 1, opt, x);
    }

    pcr_exception_unwind(ex);
}


extern void pcr_vector_muterate(pcr_vector **ctx, pcr_muterator *mtr, void *opt,
                                        pcr_exception ex)
{
    pcr_assert_handle(ctx && *ctx && mtr, ex);

    pcr_exception_try (x) {
        pcr_vector *hnd = vec_fork(ctx, x);
        for (register size_t i = 0, len = hnd->len; i < len; i++)
            mtr(hnd->payload[i], i + 1, opt, x);
    }

    pcr_exception_unwind(ex);
}

