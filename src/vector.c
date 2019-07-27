#include "./api.h"


struct pcr_vector {
    void **payload;
    size_t sz;
    size_t len;
    size_t cap;
};


extern pcr_vector *pcr_vector_new(const size_t elemsz, pcr_exception ex)
{
    pcr_assert_range(elemsz, ex);

    pcr_exception_try (x) {
        pcr_vector *ctx = pcr_mempool_alloc(sizeof *ctx, x);

        ctx->sz = elemsz;
        ctx->len = 0;
        ctx->cap = 4;
        ctx->payload = pcr_mempool_alloc(elemsz * ctx->cap, x);

        return ctx;
    }

    pcr_exception_unwind(ex);
    return NULL;
}

