#include "../lib/utf8.h"
#include "./api.h"


extern pcr_string *pcr_string_new(const unsigned char *cstr, pcr_exception ex)
{
    pcr_assert_handle(cstr, ex);

    pcr_exception_try (x) {
        const size_t len = utf8len(cstr) + 1;
        pcr_string *ctx = pcr_mempool_alloc(sizeof *ctx * len, x);

        (void) utf8cpy(ctx, cstr);
        ctx[len] = '\0';

        return ctx;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_string *pcr_string_copy(const pcr_string *ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        return pcr_string_new(ctx, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern size_t pcr_string_len(const pcr_string *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return utf8len(ctx);
}


extern pcr_string *pcr_string_add(const pcr_string *ctx, const pcr_string *add,
                                        pcr_exception ex)
{
}


extern size_t pcr_string_search(const pcr_string *ctx, const pcr_string *sub,
                                        pcr_exception ex)
{
}


extern pcr_string *pcr_string_substr(const pcr_string *ctx, size_t start,
                                        size_t end, pcr_exception ex)
{
}


extern pcr_string *pcr_string_split(const pcr_string *ctx, size_t where,
                                        pcr_exception ex)
{
}

