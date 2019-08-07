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
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        void *value = pcr_mempool_alloc(ctx->sz, x);
        memcpy(value, ctx->value, ctx->sz);

        return value;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


static pcr_field *field_fork(pcr_field **ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_field *hnd = *ctx;
        if (hnd->ref > 1) {
            hnd->ref--;
            *ctx = pcr_field_new(hnd->type, hnd->sz, hnd->key, hnd->value, x);
        }

        return *ctx;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern void pcr_field_setvalue(pcr_field **ctx, const void *value,
                                    pcr_exception ex)
{
    pcr_assert_handle(ctx && *ctx && value, ex);

    pcr_exception_try (x) {
        pcr_field *hnd = field_fork(ctx, x);
        memcpy(hnd->value, value, hnd->sz);
    }

    pcr_exception_unwind(ex);
}


extern pcr_string *
pcr_field_string(const pcr_field *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        switch (ctx->type) {
            case PCR_FIELD_INT:
                pcr_assert_handle(ctx->value, x);
                return pcr_string_parseint(*((int64_t *) ctx->value), x);
                break;

            case PCR_FIELD_FLOAT:
                pcr_assert_handle(ctx->value, x);
                return pcr_string_parsefloat(*((double *) ctx->value), x);
                break;

            case PCR_FIELD_TEXT:
                return pcr_string_copy((pcr_string *) ctx->value, x);
                break;

            default:
                return pcr_string_new("null", x);
                break;
        }
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_string *
pcr_field_json(const pcr_field *ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *value = pcr_field_string(ctx, x);

        pcr_string *json = pcr_string_new("\"", x);
        json = pcr_string_add(json, ctx->key, x);
        json = pcr_string_add(json, "\":\"", x);
        json = pcr_string_add(json, value, x);
        json = pcr_string_add(json, "\"", x);

        return json;
    }

    pcr_exception_unwind(ex);
    return NULL;
}

