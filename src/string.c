#include <inttypes.h>
#include <string.h>
#include "./api.h"


static inline bool utf8_continuation(char c) {
    return (c & 0xC0) == 0x80;
}


static inline size_t utf8_strlen(const char *str) {
    register size_t i = 0, len = 0;
    while (str[i]) {
        if (!utf8_continuation(str[i]))
            len++;
        i++;
    }

    return len;
}


/* Implement the pcr_string_new() interface function. */
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


/* Implement the pcr_string_copy() interface function. */
extern pcr_string *
pcr_string_copy(const pcr_string *ctx, pcr_exception ex)
{
    return pcr_string_new(ctx, ex);
}


/* Implement the pcr_string_len() interface function. */
extern size_t
pcr_string_len(const pcr_string *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return utf8_strlen(ctx);
}


/* Implement the pcr_string_sz() interface function. */
extern size_t
pcr_string_sz(const pcr_string *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return strlen(ctx) + 1;
}


/* Implement the pcr_string_cmp() interface function. */
extern int
pcr_string_cmp(const pcr_string *lhs, const pcr_string *rhs, pcr_exception ex)
{
    pcr_assert_handle(lhs && rhs, ex);
    return strcmp(lhs, rhs);
}


/* Implement the pcr_string_add() interface function. */
extern pcr_string *
pcr_string_add(const pcr_string *ctx, const pcr_string *add, pcr_exception ex)
{
    pcr_assert_handle(ctx && add, ex);


    pcr_exception_try (x) {
        const size_t llen = strlen(ctx) + 1;
        const size_t rlen = strlen(add) + 1;

        pcr_string *cat = pcr_mempool_alloc(sizeof *cat * (llen + rlen), x);
        (void) strncpy(cat, ctx, llen);
        return strncat(cat, add, rlen);
    }

    pcr_exception_unwind(ex);
    return NULL;
}

//1 2 3 4 5 6 7 8
//a b c d e f g h
//3     5         => 8
/* Implement the pcr_string_find() interface function. */
extern size_t
pcr_string_find(const pcr_string *haystack, const pcr_string *needle,
                pcr_exception ex)
{
    pcr_assert_handle(haystack && needle, ex);

    pcr_exception_try (x) {
        pcr_string *sub = strstr(haystack, needle);

        const size_t offset = 1;
        return sub ? (utf8_strlen(haystack) - utf8_strlen(sub) + offset) : 0;
    }

    pcr_exception_unwind(ex);
    return 0;
}


// https://gist.github.com/stanislaw/f62c36823242c4ffea1b
/* Implement the pcr_string_replace() interface function. */
extern pcr_string *
pcr_string_replace(const pcr_string *haystack, const pcr_string *needle,
                   const pcr_string *replace, pcr_exception ex)
{
    pcr_assert_handle(haystack && replace, ex);
    pcr_assert_string(needle, ex);

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
/* Implement the pcr_string_replace_all() interface function. */
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


/* Implement the pcr_string_int() interface function. */
extern pcr_string *
pcr_string_int(int64_t value, pcr_exception ex)
{
    pcr_exception_try (x) {
        size_t len = snprintf(NULL, 0, "%"PRId64, value) + 1;
        pcr_string *str = pcr_mempool_alloc(sizeof *str * len, x);
        (void) snprintf(str, len, "%"PRId64, value);

        return str;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


/* Implement the pcr_string_float() interface function. */
extern pcr_string *
pcr_string_float(double value, pcr_exception ex)
{
    pcr_exception_try (x) {
        size_t len = snprintf(NULL, 0, "%lf", value) + 1;
        pcr_string *str = pcr_mempool_alloc(sizeof *str * len, x);
        snprintf(str, len, "%lf", value);

        return str;
    }

    pcr_exception_unwind(ex);
    return NULL;
}

