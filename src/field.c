#include <string.h>
#include "./api.h"


struct pcr_field {
    void *value;
    pcr_string *key;
    PCR_FIELD type;
    size_t sz;
    size_t ref;
};


extern pcr_field *pcr_field_new(PCR_FIELD type, size_t elemsz,
                                        const pcr_string *key,
                                        const void *value, pcr_exception ex)
{
    pcr_assert_range(elemsz, ex);

    pcr_exception_try (x) {
        pcr_field *ctx = pcr_mempool_alloc(sizeof *ctx, x);

        ctx->sz = elemsz;
        ctx->ref = 1;
        ctx->type = type;
        ctx->key = pcr_string_copy(key, x);

        ctx->value = pcr_mempool_alloc(ctx->sz, x);
        memcpy(ctx->value, value, ctx->sz);

        return ctx;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_field *pcr_field_copy(const pcr_field *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_field *hnd = (pcr_field *) ctx;
    hnd->ref++;

    return hnd;
}


extern PCR_FIELD pcr_field_type(const pcr_field *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return ctx->type;
}


extern pcr_string *pcr_field_key(const pcr_field *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return pcr_string_copy(ctx->key, ex);
}


extern size_t pcr_field_refcount(const pcr_field *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return ctx->ref;
}


extern void *pcr_field_value(const pcr_field *ctx, pcr_exception ex)
{
}


extern void pcr_field_setvalue(pcr_field **ctx, const void *value,
                                    pcr_exception *ex)
{
}

