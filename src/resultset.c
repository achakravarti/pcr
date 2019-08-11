#include "./api.h"

struct pcr_resultset {
    pcr_string *name;
    pcr_string_vector *keys;
    PCR_ATTRIBUTE_VECTOR *types;
    pcr_vector *values;
    size_t ref;
};


extern pcr_resultset *
pcr_resultset_new(const pcr_string *name, const pcr_string_vector *keys,
                  const PCR_ATTRIBUTE_VECTOR *types, pcr_exception ex)
{
    pcr_assert_string(name, ex);

    pcr_exception_try (x) {
        pcr_resultset *ctx = pcr_mempool_alloc(sizeof *ctx, x);

        ctx->ref = 1;
        ctx->name = pcr_string_copy(name, x);
        ctx->keys = pcr_vector_copy(keys, x);
        ctx->types = pcr_vector_copy(types, x);
        ctx->values = pcr_vector_new(sizeof (void *), x);

        return ctx;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_resultset *
pcr_resultset_copy(const pcr_resultset *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_resultset *hnd = (pcr_resultset *) ctx;
    hnd->ref++;

    return hnd;
}


extern pcr_string_vector *
pcr_resultset_keys(const pcr_resultset *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        return pcr_vector_copy(ctx->keys, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern PCR_ATTRIBUTE_VECTOR *
pcr_resultset_types(const pcr_resultset *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        return pcr_vector_copy(ctx->types, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern size_t
pcr_resultset_rows(const pcr_resultset *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        return pcr_vector_len(ctx->values, x);
    }

    pcr_exception_unwind(ex);
    return 0;
}


extern size_t
pcr_resultset_cols(const pcr_resultset *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        return pcr_vector_len(ctx->keys, x);
    }

    pcr_exception_unwind(ex);
    return 0;
}


extern pcr_attribute *
pcr_resultset_attrib(const pcr_resultset *ctx, size_t row, size_t col,
                     pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        pcr_string *key = pcr_vector_elem(ctx->keys, col, x);
        PCR_ATTRIBUTE *type = pcr_vector_elem(ctx->types, col, x);
        void *value = pcr_vector_elem(ctx->values, row * col, x);

        return pcr_attribute_new(*type, key, value, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


static pcr_resultset *
rset_fork(pcr_resultset **ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_resultset *hnd = *ctx;

        if (hnd->ref > 1) {
            hnd->ref--;

            pcr_resultset *frk = pcr_resultset_new(hnd->name, hnd->keys,
                                                   hnd->types, x);
            frk->values = pcr_vector_copy(hnd->values, x);
        }

        return *ctx;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


static void
attrib_check(const pcr_resultset *ctx, const pcr_attribute *attr, size_t col,
             pcr_exception ex)
{
    pcr_exception_try (x) {
        if (!col) {
            size_t q = pcr_vector_len(ctx->values, x) + 1;
            size_t d = pcr_vector_len(ctx->keys, x);
            col = q % d;
        }

        pcr_string *lkey = pcr_attribute_key(attr, x);
        pcr_string *rkey = pcr_vector_elem(ctx->keys, col, x);
        pcr_assert_state(!pcr_string_cmp(lkey, rkey, x), x);

        PCR_ATTRIBUTE ltype = pcr_attribute_type(attr, x);
        PCR_ATTRIBUTE *rtype = pcr_vector_elem(ctx->types, col, x);
        pcr_assert_state(ltype == *rtype, x);
    }

    pcr_exception_unwind(ex);
}


extern void
pcr_resultset_setattrib(pcr_resultset **ctx, const pcr_attribute *attr,
                        size_t row, size_t col, pcr_exception ex)
{
    pcr_assert_handle(ctx && *ctx && attr, ex);
    attrib_check(*ctx, attr, col, ex);

    pcr_exception_try (x) {
        pcr_resultset *hnd = rset_fork(ctx, x);
        pcr_vector_setelem(&hnd->values, attr, row * col, x);
    }

    pcr_exception_unwind(ex);
}


extern void
pcr_resultset_push(pcr_resultset **ctx, const pcr_attribute *attr,
                   pcr_exception ex)
{
    pcr_assert_handle(ctx && *ctx && attr, ex);
    attrib_check(*ctx, attr, 0, ex);

    pcr_exception_try (x) {
        pcr_resultset *hnd = rset_fork(ctx, x);
        pcr_vector_push(&hnd->values, pcr_attribute_value(attr, x), x);
    }

    pcr_exception_unwind(ex);
}


extern pcr_string *
pcr_resultset_json(const pcr_resultset *ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        register size_t items = pcr_vector_len(ctx->values, x);
        register size_t cols = pcr_vector_len(ctx->keys, x);
        register size_t rows = items / cols;

        pcr_string *json = pcr_string_new("{", x);
        json = pcr_string_add(json, ctx->name, x);
        json = pcr_string_add(json, ": [", x);

        if (pcr_hint_unlikely (!rows))
            return pcr_string_add(json, "]}", x);

        pcr_attribute *attr;
        for (register size_t r = 1; r <= rows; r++) {
            register size_t c = 1;
            json = pcr_string_add(json, "{", x);

            for (; c <= cols; c++) {
                attr = pcr_resultset_attrib(ctx, r, c, x);
                json = pcr_string_add(json, pcr_attribute_json(attr, x), x);
                if (pcr_hint_unlikely(c < cols))
                    json = pcr_string_add(json, ",", x);
            }

            json = pcr_string_add(json, "}", x);
            if (pcr_hint_unlikely ((r * c) < items))
                json = pcr_string_add(json, ",", x);
        }

        return pcr_string_add(json, "]}", x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}

