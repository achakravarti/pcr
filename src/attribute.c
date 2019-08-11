#include <string.h>
#include "./api.h"


struct pcr_attribute {
    void *value;
    pcr_string *key;
    PCR_ATTRIBUTE type;
};


static size_t value_size(PCR_ATTRIBUTE type, const void *value,
                         pcr_exception ex)
{
    switch (type) {
        case PCR_ATTRIBUTE_INT:
            return sizeof (int64_t);
            break;

        case PCR_ATTRIBUTE_FLOAT:
            return sizeof (double);
            break;

        case PCR_ATTRIBUTE_TEXT:
            return pcr_string_len((pcr_string *) value, ex);
            break;

        default:
            return sizeof (void *);
            break;
    }
}


extern pcr_attribute *
pcr_attribute_new(PCR_ATTRIBUTE type, const pcr_string *key, const void *value,
                  pcr_exception ex)
{
    pcr_assert_string(key, ex);

    pcr_exception_try (x) {
        pcr_attribute *ctx = pcr_mempool_alloc(sizeof *ctx, x);

        ctx->type = type;
        ctx->key = pcr_string_copy(key, x);

        size_t sz = value_size(type, value, x);
        ctx->value = pcr_mempool_alloc(sz, x);
        memcpy(ctx->value, value, sz);

        return ctx;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern inline pcr_attribute *
pcr_attribute_copy(const pcr_attribute *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return (pcr_attribute *) ctx;
}


extern inline PCR_ATTRIBUTE
pcr_attribute_type(const pcr_attribute *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return ctx->type;
}


extern pcr_string *
pcr_attribute_key(const pcr_attribute *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        return pcr_string_copy(ctx->key, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern void *
pcr_attribute_value(const pcr_attribute *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        size_t sz = value_size(ctx->type, ctx->value, x);
        void *value = pcr_mempool_alloc(sz, x);
        memcpy(value, ctx->value, sz);

        return value;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern size_t
pcr_attribute_valuesz(const pcr_attribute *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        return value_size(ctx->type, ctx->value, x);
    }

    pcr_exception_unwind(ex);
    return 0;
}


extern pcr_string *
pcr_attribute_string(const pcr_attribute *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        switch (ctx->type) {
            case PCR_ATTRIBUTE_INT:
                pcr_assert_handle(ctx->value, x);
                return pcr_string_parseint(*((int64_t *) ctx->value), x);
                break;

            case PCR_ATTRIBUTE_FLOAT:
                pcr_assert_handle(ctx->value, x);
                return pcr_string_parsefloat(*((double *) ctx->value), x);
                break;

            case PCR_ATTRIBUTE_TEXT:
                return pcr_string_copy((pcr_string *) ctx->value, x);
                break;

            default:
                return pcr_string_new("NULL", x);
                break;
        }
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_string *
pcr_attribute_json(const pcr_attribute *ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *json = pcr_string_new("\"", x);
        json = pcr_string_add(json, ctx->key, x);
        json = pcr_string_add(json, "\":\"", x);
        json = pcr_string_add(json, pcr_attribute_string(ctx, x), x);
        json = pcr_string_add(json, "\"", x);

        return json;
    }

    pcr_exception_unwind(ex);
    return NULL;
}

