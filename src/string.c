#include <inttypes.h>
#include <string.h>
#include "../lib/utf8.h"
#include "./api.h"


extern pcr_string *
pcr_string_new(const char *cstr, pcr_exception ex)
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


extern pcr_string *
pcr_string_copy(const pcr_string *ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        return pcr_string_new(ctx, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern size_t
pcr_string_len(const pcr_string *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return utf8len(ctx);
}


extern pcr_string *
pcr_string_add(const pcr_string *ctx, const pcr_string *add, pcr_exception ex)
{
    pcr_assert_handle(ctx && add, ex);


    pcr_exception_try (x) {
        const size_t llen = utf8len(ctx);
        const size_t rlen = utf8len(add);

        pcr_string *cat = pcr_mempool_alloc(sizeof *cat * (llen + rlen + 1), x);
        (void) utf8ncpy(cat, ctx, llen);
        return utf8ncat(cat, add, rlen);
    }

    pcr_exception_unwind(ex);
    return NULL;
}

//1 2 3 4 5 6 7 8
//a b c d e f g h
//3     5         => 8
extern size_t
pcr_string_find(const pcr_string *haystack, const pcr_string *needle,
                    pcr_exception ex)
{
    pcr_assert_handle(haystack && needle, ex);

    pcr_exception_try (x) {
        pcr_string *sub = utf8str(haystack, needle);

        const size_t offset = 1;
        return sub ? (utf8len(haystack) - utf8len(sub) + offset) : 0;
    }

    pcr_exception_unwind(ex);
    return 0;
}


extern pcr_string *
pcr_string_parseint(int64_t value, pcr_exception ex)
{
    pcr_exception_try (x) {
        size_t len = snprintf(NULL, 0, "%"PRId64, value);
        pcr_assert_range(len >= 0, ex);

        pcr_string *str = pcr_mempool_alloc(sizeof *str * len, x);
        snprintf(str, len, "%"PRId64, value);

        return str;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_string *
pcr_string_parsefloat(double value, pcr_exception ex)
{
    pcr_exception_try (x) {
        size_t len = snprintf(NULL, 0, "%lf", value);
        pcr_assert_range(len >= 0, ex);

        pcr_string *str = pcr_mempool_alloc(sizeof *str * len, x);
        snprintf(str, len, "%lf", value);

        return str;
    }

    pcr_exception_unwind(ex);
    return NULL;
}
