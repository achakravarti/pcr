#include "./api.h"


extern pcr_record *
pcr_record_new(const pcr_field_vector *fields, pcr_exception ex)
{
    pcr_assert_handle(fields, ex);
    pcr_assert_state(pcr_vector_len(fields, ex), ex);

    pcr_exception_try (x) {
        return pcr_vector_copy(fields, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_record *
pcr_record_copy(const pcr_record *ctx, pcr_exception ex)
{
    return pcr_vector_copy(ctx, ex);
}


extern size_t
pcr_record_len(const pcr_record *ctx, pcr_exception ex)
{
    return pcr_vector_len(ctx, ex);
}


extern size_t
pcr_record_refcount(const pcr_record *ctx, pcr_exception ex)
{
    return pcr_vector_refcount(ctx, ex);
}


static void
keys_push(const void *elem, size_t idx, void *opt, pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string_vector *keys = (pcr_string_vector *) opt;
        pcr_vector_push(&keys, pcr_field_key((pcr_field *) elem, x), x);
    }

    pcr_exception_unwind(ex);
}


extern pcr_string_vector *
pcr_record_keys(const pcr_record *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        pcr_string_vector *keys = pcr_vector_new(sizeof (pcr_string), x);
        pcr_vector_iterate(ctx, &keys_push, &keys, x);

        return keys;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


static void
types_push(const void *elem, size_t idx, void *opt, pcr_exception ex)
{
    pcr_exception_try (x) {
        PCR_FIELD_VECTOR *types = (PCR_FIELD_VECTOR *) opt;
        PCR_FIELD type = pcr_field_type((pcr_field *) elem, x);
        pcr_vector_push(&types, &type, x);
    }

    pcr_exception_unwind(ex);
}


extern PCR_FIELD_VECTOR *
pcr_record_types(const pcr_record *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        PCR_FIELD_VECTOR *types = pcr_vector_new(sizeof (PCR_FIELD), x);
        pcr_vector_iterate(ctx, &types_push, &types, x);

        return types;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_field *
pcr_record_field(const pcr_record *ctx, size_t idx, pcr_exception ex)
{
    return pcr_vector_elem(ctx, idx, ex);
}


extern void
pcr_record_setfield(pcr_record **ctx, const pcr_field *field, size_t idx,
                            pcr_exception ex)
{
    pcr_vector_setelem(ctx, field, idx, ex);
}


extern pcr_string *
pcr_record_json(const pcr_record *ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        register size_t len = pcr_vector_len(ctx, x);

        if (pcr_hint_unlikely (!len))
            return pcr_string_new("{}", x);

        register pcr_string *json = pcr_string_new("{", x);
        register pcr_field *field;

        for (register size_t i = 1; i < len; len++) {
            field = pcr_vector_elem(ctx, i, x);
            json = pcr_string_add(json, pcr_field_json(field, x), x);
            json = pcr_string_add(json, ",", x);
        }

        field = pcr_vector_elem(ctx, len, x);
        json = pcr_string_add(json, pcr_field_json(field, x), x);

        return pcr_string_add(json, "}", x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}

