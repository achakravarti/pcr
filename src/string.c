#include <inttypes.h>
#include <string.h>
#include "../lib/utf8.h"
#include "./api.h"


extern pcr_string *
pcr_string_new(const char *cstr, pcr_exception ex)
{
    pcr_assert_handle(cstr, ex);

    pcr_exception_try (x) {
        const size_t sz = strlen(cstr) + 1;
        pcr_string *ctx = pcr_mempool_alloc(sz, x);
        (void) strncpy(ctx, cstr, sz);

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


extern int
pcr_string_cmp(const pcr_string *lhs, const pcr_string *rhs, pcr_exception ex)
{
    pcr_assert_handle(lhs && rhs, ex);
    return utf8cmp(lhs, rhs);
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


// https://gist.github.com/stanislaw/f62c36823242c4ffea1b
extern pcr_string *
pcr_string_replace(const pcr_string *haystack, const pcr_string *needle,
                        const pcr_string *replace, pcr_exception ex)
{
    pcr_assert_handle(haystack && needle && replace, ex);

    pcr_exception_try (x) {
        char *pos = strstr(haystack, needle);
        if (!pos)
            return pcr_string_copy(haystack, x);

        const size_t len = strlen(haystack);
        const size_t replen = strlen(replace);
        const size_t needlen = strlen(needle);
        const size_t diff = replen - needlen;
        pcr_string *str = pcr_mempool_alloc(sizeof *str * (len + diff + 1), x);

        size_t shifts = pos - haystack;
        memcpy(str, haystack, shifts);
        memcpy(str + shifts, replace, replen);
        memcpy(str + shifts + replen, pos + needlen, len - shifts - needlen);

        str[len + diff] = '\0';
        return str;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


// https://gist.github.com/stanislaw/f62c36823242c4ffea1b
extern pcr_string *
pcr_string_replaceall(const pcr_string *haystack, const pcr_string *needle,
                            const pcr_string *replace, pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_string *repl = pcr_string_replace(haystack, needle, replace, x);

        while (strstr(repl, needle))
            repl = pcr_string_replace(repl, needle, replace, x);

        return repl;
    }

    pcr_exception_unwind(ex);
    return NULL;
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

