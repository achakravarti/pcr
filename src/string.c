#include <inttypes.h>
#include <string.h>
#include "./api.h"


/* Define the null char symbolic constant. This offset is required in order to
 * correctly account for the offset caused by the terminating null character in
 * string instances. */

#define NULLCHAR_OFFSET 1


/* Define the utf8_continuation() helper function. This function is responsible
 * for determining whether a given character byte `c` represents a Unicode
 * continuation byte. This code is adapted from the GitHub gist available at
 * https://gist.github.com/stanislaw/f62c36823242c4ffea1b */

static inline bool utf8_continuation(char c) {
    return (c & 0xC0) == 0x80;
}


/* Define the utf8_strlen() helper function. This function is responsible for
 * determining the lexicographical length of a UTF-8 string `str`. Since UTF-8
 * characters are of variable length, the standard strlen() function cannot
 * reliably determine the lexicographical length of UTF-8 strings since it
 * assumes that each character is of one byte. */

static inline size_t utf8_strlen(const char *str) {
    register size_t i = 0, len = 0;
    while (str[i]) {
        if (!utf8_continuation(str[i]))
            len++;
        i++;
    }

    return len;
}


static pcr_string *
replace_first(const pcr_string *h, const pcr_string *n, const pcr_string *r,
              pcr_exception ex)
{
    pcr_exception_try (x) {
        char *pos = strstr(h, n);
        if (!pos)
            return pcr_string_copy(h, ex);

        const size_t hsz = strlen(h);
        const size_t nsz = strlen(n);
        const size_t rsz = strlen(r);
        const size_t diff = rsz - nsz;
        pcr_string *s = pcr_mempool_alloc(hsz + diff + NULLCHAR_OFFSET, ex);

        size_t shifts = pos - h;
        memcpy(s, h, shifts);
        memcpy(s + shifts, r, rsz);
        memcpy(s + shifts + rsz, pos + nsz, hsz - shifts - nsz);

        s[hsz + diff] = '\0';
        return s;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


/* Implement the pcr_string_new() interface function. We use the Boehm garbage
 * collector (through pcr_mempool_alloc() to manage the heap memory allocated to
 * PCR string instances. */

extern pcr_string *
pcr_string_new(const char *cstr, pcr_exception ex)
{
    pcr_assert_handle(cstr, ex);

    pcr_exception_try (x) {
        const size_t sz = strlen(cstr) + NULLCHAR_OFFSET;
        pcr_string *ctx = pcr_mempool_alloc(sz, x);
        (void) strncpy(ctx, cstr, sz);

        return ctx;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


/* Implement the pcr_string_copy() interface function. Copying is straight-
 * forward since the heap memory allocated to PCR strings is managed by the
 * Boehm garbage collector. */

extern pcr_string *
pcr_string_copy(const pcr_string *ctx, pcr_exception ex)
{
    return pcr_string_new(ctx, ex);
}


/* Implement the pcr_string_len() interface function. We can't use the standard
 * strlen() function to reliably determine the length of UTF-8 strings, and need
 * to rely on the utf8_strlen() helper function instead. */

extern size_t
pcr_string_len(const pcr_string *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return utf8_strlen(ctx);
}


/* Implement the pcr_string_sz() interface function. We use the standard
 * strlen() function to determine the number of bytes in @ctx since it works by
 * counting the actual number of bytes in a string buffer. */

extern size_t
pcr_string_sz(const pcr_string *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);
    return strlen(ctx) + 1;
}


/* Implement the pcr_string_cmp() interface function. We use the standard
 * strcmp() function to perform a byte-by-byte comparison of @lhs and @rhs. */

extern int
pcr_string_cmp(const pcr_string *lhs, const pcr_string *rhs, pcr_exception ex)
{
    pcr_assert_handle(lhs && rhs, ex);
    return strcmp(lhs, rhs);
}


/* Implement the pcr_string_add() interface function. Since we need to work with
 * the individual bytes in @ctx and @add, we can safely use the standard library
 * functions strlen(), strncpy(), and strncat() for the necessary processing. */

extern pcr_string *
pcr_string_add(const pcr_string *ctx, const pcr_string *add, pcr_exception ex)
{
    pcr_assert_handle(ctx && add, ex);

    pcr_exception_try (x) {
        const size_t llen = strlen(ctx) + NULLCHAR_OFFSET;
        const size_t rlen = strlen(add) + NULLCHAR_OFFSET;

        pcr_string *cat = pcr_mempool_alloc(sizeof *cat * (llen + rlen), x);
        (void) strncpy(cat, ctx, llen);
        return strncat(cat, add, rlen);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


/* Implement the pcr_string_find() interface function. Since UTF-8 characters
 * are of variable length, we cannot reliably determine the index where @needle
 * is found simply through the pointer returned by strstr(). Instead, we need to
 * compute the index by subtracting the lexicographical length of the substring
 * from the lexicographical length of the string, accounting for the fact that
 * pcr_string indices are 1-based. */

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


/* Implement the pcr_string_replace_first() interface function. This code is
 * adapted from https://gist.github.com/stanislaw/f62c36823242c4ffea1b. */

extern pcr_string *
pcr_string_replace_first(const pcr_string *haystack, const pcr_string *needle,
                         const pcr_string *replace, pcr_exception ex)
{
    pcr_assert_handle(haystack && replace, ex);
    pcr_assert_string(needle, ex);

    return replace_first(haystack, needle, replace, ex);
}


// https://gist.github.com/stanislaw/f62c36823242c4ffea1b
/* Implement the pcr_string_replace_all() interface function. */

extern pcr_string *
pcr_string_replace(const pcr_string *haystack, const pcr_string *needle,
                   const pcr_string *replace, pcr_exception ex)
{
    pcr_assert_handle(haystack && replace, ex);
    pcr_assert_string(needle, ex);

    pcr_exception_try (x) {
        register pcr_string *r;
        if (pcr_hint_likely (!strstr(replace, needle))) {
            r = replace_first(haystack, needle, replace, x);
            while (strstr(r, needle))
                r = replace_first(r, needle, replace, x);

            return r;
        }

        const char placeholder[] = {0x1, 0x0};
        r = replace_first(haystack, needle, placeholder, x);
        while (strstr(r, needle))
            r = replace_first(r, needle, placeholder, x);

        r = replace_first(r, placeholder, replace, x);
        while (strstr(r, placeholder))
            r = replace_first(r, placeholder, replace, x);

        return r;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


/* Implement the pcr_string_int() interface function. We use the standard
 * snprintf() function for both determining the number of bytes required to hold
 * the string representation of @value, and also to actually generate the string
 * representation. */

extern pcr_string *
pcr_string_int(int64_t value, pcr_exception ex)
{
    pcr_exception_try (x) {
        size_t len = snprintf(NULL, 0, "%"PRId64, value) + NULLCHAR_OFFSET;
        pcr_string *str = pcr_mempool_alloc(sizeof *str * len, x);
        (void) snprintf(str, len, "%"PRId64, value);

        return str;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


/* Implement the pcr_string_float() interface function. We use the standard
 * snprintf() function to both determine the number of bytes required to hold
 * the string representation of @value, and also to actually generate the string
 * representation. */

extern pcr_string *
pcr_string_float(double value, pcr_exception ex)
{
    pcr_exception_try (x) {
        size_t len = snprintf(NULL, 0, "%lf", value) + NULLCHAR_OFFSET;
        pcr_string *str = pcr_mempool_alloc(sizeof *str * len, x);
        snprintf(str, len, "%lf", value);

        return str;
    }

    pcr_exception_unwind(ex);
    return NULL;
}

