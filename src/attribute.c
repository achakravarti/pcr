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
            return pcr_string_sz((pcr_string *) value, ex);
            break;

        default:
            return 0;
            break;
    }
}


extern pcr_attribute *
pcr_attribute_new(PCR_ATTRIBUTE type, const pcr_string *key, const void *value,
                  pcr_exception ex)
{
    pcr_assert_string(key, ex);
    if (pcr_hint_likely (type != PCR_ATTRIBUTE_NULL))
        pcr_assert_handle(value, ex);

    pcr_exception_try (x) {
        pcr_attribute *ctx = pcr_mempool_alloc(sizeof *ctx, x);

        ctx->type = type;
        ctx->key = pcr_string_copy(key, x);

        size_t sz = value_size(type, value, x);
        if (pcr_hint_likely (sz)) {
            ctx->value = pcr_mempool_alloc(sz, x);
            memcpy(ctx->value, value, sz);
        }

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
        void *value = NULL;
        size_t sz = value_size(ctx->type, ctx->value, x);

        if (pcr_hint_likely (sz)) {
            value = pcr_mempool_alloc(sz, x);
            memcpy(value, ctx->value, sz);
        }

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
                return pcr_string_int(*((int64_t *) ctx->value), x);
                break;

            case PCR_ATTRIBUTE_FLOAT:
                pcr_assert_handle(ctx->value, x);
                return pcr_string_float(*((double *) ctx->value), x);
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
    pcr_assert_handle(ctx, ex);

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


/*******************************************************************************
 * pcr_attribute Inline Declarations
 */


extern inline pcr_attribute *
pcr_attribute_new_null(const pcr_string *key, pcr_exception ex);


extern inline pcr_attribute *
pcr_attribute_new_int(const pcr_string *key, int64_t value, pcr_exception ex);


extern inline pcr_attribute *
pcr_attribute_new_int_2(const pcr_string *key, pcr_exception ex);


extern inline pcr_attribute *
pcr_attribute_new_float(const pcr_string *key, double value, pcr_exception ex);


extern inline pcr_attribute *
pcr_attribute_new_float_2(const pcr_string *key, pcr_exception ex);


extern inline pcr_attribute *
pcr_attribute_new_text(const pcr_string *key, const pcr_string *value,
                       pcr_exception ex);


extern inline pcr_attribute *
pcr_attribute_new_text_2(const pcr_string *key, pcr_exception ex);


/*******************************************************************************
 * PCR_ATTRIBUTE_VECTOR Inline Declarations
 */


extern inline PCR_ATTRIBUTE_VECTOR *
PCR_ATTRIBUTE_VECTOR_NEW(pcr_exception ex);


extern inline PCR_ATTRIBUTE_VECTOR *
PCR_ATTRIBUTE_VECTOR_NEW_2(const PCR_ATTRIBUTE *arr, size_t len,
                           pcr_exception ex);


extern inline PCR_ATTRIBUTE_VECTOR *
PCR_ATTRIBUTE_VECTOR_COPY(const PCR_ATTRIBUTE_VECTOR *ctx, pcr_exception ex);


extern inline size_t
PCR_ATTRIBUTE_VECTOR_LEN(const PCR_ATTRIBUTE_VECTOR *ctx, pcr_exception ex);


extern inline size_t
PCR_ATTRIBUTE_VECTOR_REFCOUNT(const PCR_ATTRIBUTE_VECTOR *ctx,
                              pcr_exception ex);


extern inline PCR_ATTRIBUTE
PCR_ATTRIBUTE_VECTOR_ELEM(const PCR_ATTRIBUTE_VECTOR *ctx, size_t idx,
                          pcr_exception ex);


extern inline void
PCR_ATTRIBUTE_VECTOR_ELEM_SET(PCR_ATTRIBUTE_VECTOR **ctx, size_t idx,
                              PCR_ATTRIBUTE elem, pcr_exception ex);


extern inline void
PCR_ATTRIBUTE_VECTOR_PUSH(PCR_ATTRIBUTE_VECTOR **ctx, PCR_ATTRIBUTE elem,
                          pcr_exception ex);


extern inline void
PCR_ATTRIBUTE_VECTOR_ITERATE(const PCR_ATTRIBUTE_VECTOR *ctx, pcr_iterator *itr,
                             void *opt, pcr_exception ex);


extern inline void
PCR_ATTRIBUTE_VECTOR_MUTERATE(PCR_ATTRIBUTE_VECTOR **ctx, pcr_muterator *mtr,
                              void *opt, pcr_exception ex);


/*******************************************************************************
 * pcr_attribute_vector Inline Declarations
 */


extern inline pcr_attribute_vector *
pcr_attribute_vector_new(pcr_exception ex);


extern inline pcr_attribute_vector *
pcr_attribute_vector_new_2(const pcr_attribute **arr, size_t len,
                           pcr_exception ex);


extern inline pcr_attribute_vector *
pcr_attribute_vector_copy(const pcr_attribute_vector *ctx, pcr_exception ex);


extern inline size_t
pcr_attribute_vector_len(const pcr_attribute_vector *ctx, pcr_exception ex);


extern inline size_t
pcr_attribute_vector_refcount(const pcr_string_vector *ctx, pcr_exception ex);


extern inline pcr_attribute *
pcr_attribute_vector_elem(const pcr_attribute_vector *ctx, size_t idx,
                          pcr_exception ex);


extern inline void
pcr_attribute_vector_elem_set(pcr_attribute_vector **ctx, size_t idx,
                              const pcr_attribute *elem, pcr_exception ex);


extern inline void
pcr_attribute_vector_push(pcr_attribute_vector **ctx, const pcr_attribute *elem,
                          pcr_exception ex);


extern inline void
pcr_attribute_vector_iterate(const pcr_attribute_vector *ctx, pcr_iterator *itr,
                             void *opt, pcr_exception ex);


extern inline void
pcr_attribute_vector_muterate(pcr_attribute_vector **ctx, pcr_muterator *mtr,
                              void *opt, pcr_exception ex);

