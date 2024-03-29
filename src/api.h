#if !defined PROTO_C_RUNTIME
#define PROTO_C_RUNTIME

#if defined (__cplusplus)
extern "C" {
#endif


/**************************************************************************//**
 * @defgroup hint PCR Hint Module
 * Compiler hints for optimisation.
 *
 * Compilers such as GCC have always focused aggressively on optimisation. Many
 * such optimisations have been introduced as compiler-specific extensions that
 * are @b not part of the C standards.
 *
 * The PCR Hint Module abstracts such optimisations as conditional macros so
 * that they can be taken advantage of in GCC and GCC-compatible compilers such
 * as Clang, but at the same time degrade safely on other compilers.
 * @{
 */


/**
 * Hints that a function is pure.
 *
 * The @c pcr_hint_pure macro is used to decorate the declaration of a pure
 * function. A pure function is one that always returns the same value for the
 * same arguments without any side effects. This hint follows the GCC attribute
 * syntax, and is recommended to be placed in the function declaration just
 * before the function return type.
 *
 * @warning This hint is available only on GCC and GCC-compatible compilers such
 * as Clang; on other compilers, this macro degrades with a warning that it has
 * no effect.
 */
#if (defined __GNUC__ || defined __clang__)
#   define pcr_hint_pure __attribute__((pure))
#else
#   define pcr_hint_pure
#   warning pcr_hint_pure has no effect on non GCC-compatible compilers
#endif


/**
 * Hints that a function is called frequently.
 *
 * The @c pcr_hint_hot macro is used to decorate a function declaration in
 * order to indicate that it is called frequently. This hint follows the GCC
 * attribute syntax, and is recommended to be placed in the function declaration
 * just before the function return type.
 *
 * @warning This hint is available only on GCC and GCC-compatible compilers such
 * as Clang; on other compilers, this macro degrades with a warning that it has
 * no effect.
 *
 * @see pcr_hint_cold
 */
#if (defined __GNUC__ || defined __clang__)
#   define pcr_hint_hot __attribute__((hot))
#else
#   define pcr_hint_hot
#   warning pcr_hint_hot has no effect on non GCC-compatible compilers
#endif


/**
 * Hints that a function is called infrequently.
 *
 * The @c pcr_hint_cold macro is used to decorate a function declaration in
 * order to indicate that is called infrequently. This hint follows the GCC
 * attribute syntax, and is recommended to be placed in the function declaration
 * just before the function return type.
 *
 * @warning This hint is available only on GCC and GCC-compatible compilers such
 * as Clang; on other compilers, this macro degrades with a warning that it has
 * no effect.
 *
 * @see pcr_hint_hot
 */
#if (defined __GNUC__ || defined __clang__)
#   define pcr_hint_cold __attribute__((cold))
#else
#   define pcr_hint_cold
#   warning pcr_hint_cold has no effect on non GCC-compatible compilers
#endif


/**
 * Hints that a predicate is likely to be true.
 *
 * The @c pcr_hint_likely() macro indicates that a predicate expression @p p is
 * @b likely to evaluate to @c true. This hint has been modelled after the @c
 * likely() macro used in the Linux kernel source.
 *
 * @param p predicate expected to be true.
 *
 * @warning This hint is available only on GCC and GCC-compatible compilers such
 * as Clang; on other compilers, this macro degrades with a warning that it has
 * no effect.
 *
 * @see pcr_hint_unlikely()
 */
#if (defined __GNUC__ || defined __clang__)
#   define pcr_hint_likely(p) (__builtin_expect(!!(p), 1))
#else
#   define pcr_hint_likely
#   warning pcr_hint_likely has no effect on non GCC-compatible compilers
#endif


/**
 * Hints that a predicate is likely to be false.
 *
 * The @c pcr_hint_unlikely() macro indicates that a predicate expression @p p
 * is @b unlikely to evaluate to @c false. This hint has been modelled after the
 * @c unlikely() macro used in the Linux kernel source.
 *
 * @param p predicate expected to be false.
 *
 * @warning This hint is available only on GCC and GCC-compatible compilers such
 * as Clang; on other compilers, this macro degrades with a warning that it has
 * no effect.
 *
 * @see pcr_hint_likely()
 */
#if (defined __GNUC__ || defined __clang__)
#   define pcr_hint_unlikely(p) (__builtin_expect(!!(p), 0))
#else
#   define pcr_hint_unlikely
#   warning pcr_hint_unlikely has no effect on non GCC-compatible compilers
#endif


/**
 * @example hint.h
 * This is an example showing how to code against the PCR Hint Module interface.
 * @}
 */


/**************************************************************************//**
 * @defgroup logging PCR Logging Module
 * Runtime logging of events.
 *
 * Logging is an essential component for any sizable application. The PCR
 * Logging Module provides an interface for writing timestamped log messages.
 * These messages can be decorated as trace, warning or error messages in order
 * to make the context apparent and the message easily grep-able.
 *
 * The logging module attempts to be as thread-safe as possible, but this has
 * not been confirmed as yet. The logging module has been intentionally designed
 * to not throw any exceptions, and instead, handle failure conditions either
 * silently, or at most, with a warning message printed on to @c stdout.
 * @{
 */


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


/**
 * Opens default log file.
 *
 * The @c pcr_log_open() function opens a log file with a given fully qualified
 * @p path where all log messages will be saved by default. In case the log file
 * specified by @p path does not exist, then it will be created. If the log file
 * is opened with the flag @p flush set to @c true, then any existing data in
 * the log file will be erased. Conversely, if the flag @p flush is set to be @c
 * false, theny any existing data in the log file will be retained, and the new
 * log messages will be appended after them.
 *
 * @param path Fully qualifed path of log file.
 * @param flush Flag to flush log file on opening.
 *
 * @note There is no need to call @c pcr_log_allow() after calling this function
 * as @c pcr_log_open() allows logging by default. Also note that calling the @c
 * pcr_log_open() function is safe even if a default log file is currently open;
 * in such a case, the currently open log file will be first closed before the
 * new one is opened. In the unlikely event of @c pcr_log_open() failing to open
 * the log file, then an appropriate warning is printed onto @c stdout.
 *
 * @warning Be sure to close the log file opened with @c pcr_log_open() once you
 * are done with it by making a call to @c pcr_log_close() in order to prevent
 * resource leaks. Unlike dynamically allocated pointers on the heap, the PCR
 * Library does not manage the automatic clean-up of files.
 *
 * @see pcr_log_allow()
 * @see pcr_log_close()
 */
extern pcr_hint_cold void
pcr_log_open(const char *path, bool flush);


/**
 * Closes default log file.
 *
 * The @c pcr_log_close() function closes the default log file that was opened
 * earlier by a call to @c pcr_log_open(). Be sure to call this function after
 * you are done using the log file in order to prevent resource leaks.
 *
 * @note It is safe to call this function even if the default log file has not
 * been successfully opened. This is an intended feature to allow this function
 * to be safely called in case an exception has occurred.
 *
 * @warning Unlike dynamically allocated pointers on the heap, the PCR Library
 * does not manage the automatic clean-up of files.
 *
 * @see pcr_log_open()
 */
extern pcr_hint_cold void
pcr_log_close(void);


/**
 * Toggles on logging.
 *
 * The @c pcr_log_allow() function toggles on logging if it has been temporarily
 * disabled by a call to @c pcr_log_suppress().
 *
 * @note A call to pcr_log_open() implies that logging is allowed by default.
 *
 * @see pcr_log_open()
 * @see pcr_log_suppress()
 */
extern pcr_hint_cold void
pcr_log_allow(void);


/**
 * Toggles off logging.
 *
 * The @c pcr_log_suppress() function temporarily disables the logging of
 * messages. There aren't many reasons why you would want to do this in the
 * normal course of execution. However, it is useful to suppress log messages
 * while running unit tests that check for some exception having been raised; in
 * such a case, it prevents the log file from being cluttered with superfluous
 * error messages.
 *
 * @warning Be sure to enable logging by calling @c pcr_log_allow() when you no
 * longer need to suppress log messages. Although you could also achieve this by
 * re-opening the log file through a call to @c pcr_log_open(), it is not the
 * preferred way to do so.
 *
 * @see pcr_log_allow()
 */
extern void
pcr_log_suppress(void); // TODO: why does pcr_hint_cold cause an error?


/**
 * @private
 * Private helper function for the logging macros defined below.
 */
extern void
pcr_log_write__(const char, const char *, ...);


/**
 * Logs trace message.
 *
 * The @c pcr_log_trace() macro writes a formatted trace message @p m in the
 * currently open log file. The message is decorated with the current local
 * system time and a [T] prefix to make it easily grep-able.
 *
 * @param m Formatted message to log.
 * @param ... Format tokens.
 *
 * @note It is safe to call this macro even if the log file has not been opened
 * by an earlier call to @c pcr_log_open(); however, no message will be logged.
 * Also note that calling this macro has no effect in case logging has been
 * suppressed by a call to @c pcr_log_suppress().
 *
 * @see pcr_log_warning()
 * @see pcr_log_error()
 */
#define pcr_log_trace(m, ...) \
    pcr_log_write__('T', (m), ##__VA_ARGS__)


/**
 * Logs warning message.
 *
 * The @c pcr_log_warning() macro wirtes a formatted warning message @p m in the
 * currently open log file. The message is decorated with the current local
 * system time and a [W] prefix to make it easily grep-able.
 *
 * @param m Formatted message to log.
 * @param ... Format tokens.
 *
 * @note It is safe to call this macro even if the log file has not been opened
 * by an earlier call to @c pcr_log_open(); however, no message will be logged.
 * Also note that calling this macro has no effect in case logging has been
 * suppressed by a call to @c pcr_log_suppress().
 *
 * @see pcr_log_trace()
 * @see pcr_log_error()
 */
#define pcr_log_warning(m, ...) \
    pcr_log_write__('W', (m), ##__VA_ARGS__)


/**
 * Logs error message.
 *
 * The @c pcr_log_error() macro wirtes a formatted error message @p m in the
 * currently open log file. The message is decorated with the current local
 * system time and a [E] prefix to make it easily grep-able.
 *
 * @param m Formatted message to log.
 * @param ... Format tokens.
 *
 * @note It is safe to call this macro even if the log file has not been opened
 * by an earlier call to @c pcr_log_open(); however, no message will be logged.
 * Also note that calling this macro has no effect in case logging has been
 * suppressed by a call to @c pcr_log_suppress().
 *
 * @see pcr_log_trace()
 * @see pcr_log_warning()
 */
#define pcr_log_error(m, ...) \
    pcr_log_write__('E', (m), ##__VA_ARGS__)

/**
 * @example log.h
 * This is an example showing how to code against the PCR Logging Module
 * interface.
 * @}
 */


/******************************************************************************
 * INTERFACE: pcr_exception
 */

#include <setjmp.h>

typedef jmp_buf pcr_exception;
typedef int PCR_EXCEPTION;

#define PCR_EXCEPTION_NONE 0x0
#define PCR_EXCEPTION_HANDLE 0x1
#define PCR_EXCEPTION_STATE 0x2
#define PCR_EXCEPTION_RANGE 0x3
#define PCR_EXCEPTION_MEMPOOL 0x4
#define PCR_EXCEPTION_STRING 0x5
#define PCR_EXCEPTION_FILE 0x6
#define PCR_EXCEPTION_PARSE 0x7

#define pcr_exception_try(x)                        \
    pcr_exception x;                                \
    register PCR_EXCEPTION pcr__exid__ = setjmp(x); \
    if (pcr_hint_likely (!pcr__exid__))

#define pcr_exception_catch(x) \
    if (pcr_hint_unlikely (pcr__exid__ == (x)))

#define pcr_exception_catchall \
    if (pcr_hint_unlikely (pcr__exid__))

#define pcr_exception_throw(x, id) \
    do {                                                         \
        pcr_log_error("exception 0x%x thrown in %s() [%s:%d]",   \
                            (id), __func__, __FILE__, __LINE__); \
        longjmp((x), (id));                                      \
    } while (0)

#define pcr_exception_log()                                  \
    pcr_log_error("exception 0x%x detected in %s() [%s:%d]", \
                        pcr__exid__, __func__, __FILE__, __LINE__);

#define pcr_exception_print()                               \
    printf("[!] exception 0x%x detected in %s() [%s:%d]\n", \
                pcr__exid__, __func__, __FILE__, __LINE__);

#define pcr_exception_unwind(x)    \
    if (pcr__exid__) {             \
        pcr_exception_log();       \
        longjmp((x), pcr__exid__); \
    }                              \


/******************************************************************************
 * INTERFACE: pcr_assert
 */

#define pcr_assert_generic(p, x, id)    \
    if (pcr_hint_unlikely (!(p))) {     \
        pcr_exception_throw((x), (id)); \
    }

#define pcr_assert_handle(p, x) \
    pcr_assert_generic((p), (x), PCR_EXCEPTION_HANDLE)

#define pcr_assert_string(s, x) \
    pcr_assert_generic((s) && *(s), (x), PCR_EXCEPTION_STRING)

#define pcr_assert_state(p, x) \
    pcr_assert_generic((p), (x), PCR_EXCEPTION_STATE)

#define pcr_assert_range(p, x) \
    pcr_assert_generic((p), (x), PCR_EXCEPTION_RANGE)

#define pcr_assert_file(p, x) \
    pcr_assert_generic((p), (x), PCR_EXCEPTION_FILE)

#define pcr_assert_parse(p, x) \
    pcr_assert_generic((p), (x), PCR_EXCEPTION_PARSE)


/******************************************************************************
 * INTERFACE: pcr_mempool
 */

extern void *
pcr_mempool_alloc(size_t sz, pcr_exception ex);

extern void *
pcr_mempool_realloc(void *ptr, size_t sz, pcr_exception ex);


/******************************************************************************
 * INTERFACE: pcr_vector
 */

typedef struct pcr_vector pcr_vector;

typedef void
(pcr_iterator)(const void *elem, size_t idx, void *opt, pcr_exception ex);

typedef void
(pcr_muterator)(void *elem, size_t idx, void *opt, pcr_exception ex);

typedef int
(pcr_comparator)(const void *ctx, const void *cmp);

extern pcr_vector *
pcr_vector_new(size_t elemsz, pcr_exception ex);

extern pcr_vector *
pcr_vector_copy(const pcr_vector *ctx, pcr_exception ex);

extern size_t
pcr_vector_len(const pcr_vector *ctx, pcr_exception ex);

extern size_t
pcr_vector_refcount(const pcr_vector *ctx, pcr_exception ex);

extern bool
pcr_vector_sorted(const pcr_vector *ctx, pcr_exception ex);

extern void *
pcr_vector_elem(const pcr_vector *ctx, size_t idx, pcr_exception ex);

extern void
pcr_vector_setelem(pcr_vector **ctx, const void *elem, size_t idx,
                        pcr_exception ex);

extern void
pcr_vector_push(pcr_vector **ctx, const void *elem, pcr_exception ex);

extern void
pcr_vector_pop(pcr_vector **ctx, pcr_exception ex);

extern void
pcr_vector_sort(pcr_vector **ctx, pcr_comparator *cmp, pcr_exception ex);

extern size_t
pcr_vector_search(pcr_vector **ctx, const void *key, pcr_comparator *cmp,
                        pcr_exception ex);

extern void
pcr_vector_iterate(const pcr_vector *ctx, pcr_iterator *itr, void *opt,
                        pcr_exception ex);

extern void
pcr_vector_muterate(pcr_vector **ctx, pcr_muterator *mtr, void *opt,
                        pcr_exception ex);


/**************************************************************************//**
 * @defgroup pcr_string PCR String Module
 * UTF-8 string management.
 *
 * UTF-8 is the de-facto character representation used nowadays across the globe
 * in computing contexts. In contrast to other Unicode schemes (such as UTF-16),
 * UTF-8 uses variable bytes sizes as required for Unicode code points, the
 * smallest being 1 byte. This makes it both efficient and backward compatible
 * with ASCII.
 *
 * The PCR String Module takes this into account, and uses UTF-8 as its encoding
 * scheme. This module provides the most important string manipulation routines,
 * and its interface is expected to grow over time.
 * @{
 */


/**
 * UTF-8 string.
 *
 * The pcr_string type represents a UTF-8 string. A pcr_string may be used
 * interchangably with a raw @c char string in most (but not all) cases. The
 * heap memory allocated to PCR string instances is managed internally through
 * the Boehm Garbage Collector.
 */
typedef char pcr_string;


/**
 * Creates a new string.
 *
 * The pcr_string_new() function creates a new instance of a PCR string from a
 * raw C string @p cstr. The raw string @p cstr may either be an ASCII string or
 * a UTF-8 string.
 *
 * @param cstr The raw C string.
 * @param ex The exception stack.
 *
 * @return The new string.
 *
 * @note Since PCR strings and raw C strings may be used interchangably in most
 * (but not all) cases, it is also possible to create a new PCR string using the
 * pcr_string_copy() function.
 *
 * @warning Don't call the standard @c free() function on the string instance
 * created by this function; the heap memory allocated to the string is managed
 * internally by the PCR Library through the Boehm Garbage Collector.
 *
 * @see pcr_string_copy()
 */
extern pcr_string *
pcr_string_new(const char *cstr, pcr_exception ex);


/**
 * Clones an existing string.
 *
 * The pcr_string_copy() function creates an identical copy of an existing PCR
 * string @p ctx.
 *
 * @param ctx The contextual string instance.
 * @param ex The exception stack.
 *
 * @return The copy of @p ctx.
 *
 * @note Since PCR strings and raw C strings may be used interchangably in most
 * (but not all) cases, it is also possible to create a copy of @p ctx by using
 * the pcr_string_new() function.
 *
 * @warning Don't call the standard @c free() function on the string instance
 * created by this function; the heap memory allocated to the string is managed
 * internally by the PCR Library through the Boehm Garbage Collector.
 */
extern pcr_string *
pcr_string_copy(const pcr_string *ctx, pcr_exception ex);


/**
 * Gets string length.
 *
 * The pcr_string_len() function computes the lexicographical length of a PCR
 * string @p ctx. The lexicographical length is the number of characters in @p
 * ctx, and this function correctly accounts for the variable size of UTF-8
 * characters.
 *
 * @param ctx The contextual string instance.
 * @param ex The exception stack.
 *
 * @return The length of @p ctx.
 *
 * @warning There are as many bytes as there are characters only in ASCII
 * strings, and this is not necessarily true for UTF-8 strings. Use the
 * pcr_string_sz() function instead if you need to compute the number of bytes
 * in @p ctx.
 *
 * @see pcr_string_sz()
 */
extern size_t
pcr_string_len(const pcr_string *ctx, pcr_exception ex);


/**
 * Gets string size.
 *
 * The pcr_string_sz() function computes the size in bytes of a PCR string @p
 * ctx, @b including its terminating null character.
 *
 * @param ctx The contextual string instance.
 * @param ex The exception stack.
 *
 * @return The size of @p ctx.
 *
 * @warning There are as many bytes as there are characters only in ASCII
 * strings, and this is not necessarily true fro UTF-8 strings. Use the
 * pcr_string_len() function instead if you need to compute the lexicographical
 * length of @p ctx.
 *
 * @see pcr_string_len()
 */
extern size_t
pcr_string_sz(const pcr_string *ctx, pcr_exception ex);


/**
 * Compares two strings.
 *
 * The pcr_string_cmp() function performs a lexicographical comparison of two
 * pcr_string instances @p lhs and @p rhs. This function uses the same return
 * values as the standard @c strcmp() function to indicate the comparison
 * result.
 *
 * @param lhs The left hand side string to compare.
 * @param rhs The right hand side string to compare.
 * @param ex The exception stack.
 *
 * @return -1 if @p lhs < @p rhs.
 * @return 0 if @p == @p rhs.
 * @return 1 if @p lhs > @p rhs.
 *
 * @note The lexicographical comparison is case sensitive, so "HELLO" < "hello".
 */
extern int
pcr_string_cmp(const pcr_string *lhs, const pcr_string *rhs, pcr_exception ex);


/**
 * Concatenates two strings.
 *
 * The pcr_string_add() function creates a new string instance that is made by
 * concatenating @p add on to @p ctx.
 *
 * @param ctx The contextual string instance.
 * @param ex The exception stack.
 *
 * @return The concatenated string.
 *
 * @warning Don't call the standard @c free() function on the string instance
 * created by this function; the heap memory allocated to the string is managed
 * internally by the PCR Library through the Boehm Garbage Collector.
 */
extern pcr_string *
pcr_string_add(const pcr_string *ctx, const pcr_string *add, pcr_exception ex);


/**
 * Searches for a substring.
 *
 * The pcr_string_find() function searches for the first instance of a substring
 * @p needle in a string @p haystack, and returns the lexicographical 1-based
 * index where the substring @p needle was first found. In case @p needle is not
 * found, then 0 is returned by this function.
 *
 * @param haystack The string to search in.
 * @param needle The substring to find.
 * @param ex The exception stack.
 *
 * @return If not found, 0.
 * @return If found, the 1-based index of @p needle in @p haystack.
 */
extern size_t
pcr_string_find(const pcr_string *haystack, const pcr_string *needle,
                pcr_exception ex);


/**
 * Replaces first instance of substring.
 *
 * The pcr_string_replace_first() function replaces the first instace of a
 * substring @p needle in a string @p haystack with another substring @p
 * replace. The origin string @p haystack is not affected, and a new string with
 * the necessary replacement is returned. In case there is no replacement to be
 * made, then a copy of @p haystack is returned.
 *
 * @param haystack The string to search in.
 * @param needle The substring to replace.
 * @param replace The replacement string.
 * @param ex The exception stack.
 *
 * @return The replaced string.
 *
 * @warning Don't call the standard @c free() function on the string instance
 * created by this function; the heap memory allocated to the string is managed
 * internally by the PCR Library through the Boehm Garbage Collector.
 */
extern pcr_string *
pcr_string_replace_first(const pcr_string *haystack, const pcr_string *needle,
                         const pcr_string *replace, pcr_exception ex);


/**
 * Replaces all instances of substring.
 *
 * The pcr_string_replace() function replaces all instances of a substring @p
 * needle in a string @p haystack with another substring @p replace. The
 * original string @p haystack is not affected, and a new string with the
 * necessary replacements is returned. In case there is no replacement to be
 * made, then a copy of @p haystack is returned.
 *
 * @param haystack The string to search in.
 * @param needle The substring to replace.
 * @param replace The replacement string.
 * @param ex The exception stack.
 *
 * @return The replaced string.
 *
 * @warning Don't call the standard @c free() function on the string instance
 * created by this function; the heap memory allocated to the string is managed
 * internally by the PCR Library through the Boehm Garbage Collector.
 */
extern pcr_string *
pcr_string_replace(const pcr_string *haystack, const pcr_string *needle,
                   const pcr_string *replace, pcr_exception ex);


/**
 * Converts interger to string.
 *
 * The pcr_string_int() function generates the string representation of an
 * integer @p value.
 *
 * @param value Integer to convert.
 * @param ex The exception stack.
 *
 * @return The string representation of @p value.
 *
 * @warning Don't call the standard @c free() function on the string instance
 * created by this function; the heap memory allocated to the string is managed
 * internally by the PCR Library through the Boehm Garbage Collector.
 */
extern pcr_string *
pcr_string_int(int64_t value, pcr_exception ex);


/**
 * Converts floating point number to string.
 *
 * The pcr_string_float() function generates the string representation of a
 * floating point number @p value.
 *
 * @param value Floating point number to convert.
 * @param ex The exception stack.
 *
 * @return The string representation of @p value.
 *
 * @warning Don't call the standard @c free() function on the string instance
 * created by this function; the heap memory allocated to the string is managed
 * internally by the PCR Library through the Boehm Garbage Collector.
 */
extern pcr_string *
pcr_string_float(double value, pcr_exception ex);


/**
 * @example string.h
 * This is an example showing how to code against the PCR String Module
 * interface.
 * @}
 */


/******************************************************************************
 * INTERFACE: pcr_string_vector
 */

typedef pcr_vector pcr_string_vector;

inline pcr_string_vector *
pcr_string_vector_new(pcr_exception ex)
{
    return pcr_vector_new(sizeof (pcr_string *), ex);
}

inline pcr_string_vector *
pcr_string_vector_new_2(const pcr_string **arr, size_t len, pcr_exception ex)
{
    pcr_assert_handle(arr, ex);
    pcr_assert_range(len, ex);

    pcr_string_vector *vec = pcr_string_vector_new(ex);

    register pcr_string *str;
    for (register size_t i = 0; i < len; i++) {
        str = pcr_string_new(arr[i], ex);
        pcr_vector_push(&vec, str, ex);
    }

    return vec;
}

inline pcr_string_vector *
pcr_string_vector_copy(const pcr_string_vector *ctx, pcr_exception ex)
{
    return pcr_vector_copy(ctx, ex);
}

inline size_t
pcr_string_vector_len(const pcr_string_vector *ctx, pcr_exception ex)
{
    return pcr_vector_len(ctx, ex);
}

inline size_t
pcr_string_vector_refcount(const pcr_string_vector *ctx, pcr_exception ex)
{
    return pcr_vector_refcount(ctx, ex);
}

inline bool
pcr_string_vector_sorted(const pcr_string_vector *ctx, pcr_exception ex)
{
    return pcr_vector_sorted(ctx, ex);
}

inline pcr_string *
pcr_string_vector_elem(const pcr_string_vector *ctx, size_t idx,
                       pcr_exception ex)
{
    return (pcr_string *) pcr_vector_elem(ctx, idx, ex);
}

inline void
pcr_string_vector_elem_set(pcr_string_vector **ctx, size_t idx,
                           const pcr_string *elem, pcr_exception ex)
{
    pcr_string *str = pcr_string_new(elem, ex);
    pcr_vector_setelem(ctx, &str, idx, ex);
}

inline void
pcr_string_vector_push(pcr_string_vector **ctx, const pcr_string *elem,
                       pcr_exception ex)
{
    pcr_string *str = pcr_string_new(elem, ex);
    pcr_vector_push(ctx, &str, ex);
}

inline int
__pcr_string_vector_comparator(const void *ctx, const void *cmp)
{
    pcr_exception_try (x) {
        return !pcr_string_cmp((pcr_string *) ctx, (pcr_string *) cmp, x);
    }

    return -1;
}

inline void
pcr_string_vector_sort(pcr_string_vector **ctx, pcr_exception ex)
{
    pcr_vector_sort(ctx, &__pcr_string_vector_comparator, ex);
}

inline size_t
pcr_string_vector_search(pcr_string_vector **ctx, const pcr_string *key,
                         pcr_exception ex)
{
    return pcr_vector_search(ctx, key, &__pcr_string_vector_comparator, ex);
}

inline void
pcr_string_vector_iterate(const pcr_string_vector *ctx, pcr_iterator *itr,
                          void *opt, pcr_exception ex)
{
    pcr_vector_iterate(ctx, itr, opt, ex);
}

inline void
pcr_string_vector_muterate(pcr_string_vector **ctx, pcr_muterator *mtr,
                           void *opt, pcr_exception ex)
{
    pcr_vector_muterate(ctx, mtr, opt, ex);
}


/******************************************************************************
 * INTERFACE: pcr_testcase
 */

typedef bool
(pcr_unittest)(pcr_string **desc, pcr_exception ex);

typedef struct pcr_testcase pcr_testcase;

extern pcr_testcase *
pcr_testcase_new(pcr_unittest *test, pcr_exception ex);

extern pcr_testcase *
pcr_testcase_copy(const pcr_testcase *ctx, pcr_exception ex);

extern bool
pcr_testcase_run(pcr_testcase *ctx, pcr_exception ex);


/******************************************************************************
 * INTERFACE: pcr_testsuite
 */

typedef struct pcr_testsuite pcr_testsuite;

extern pcr_testsuite *
pcr_testsuite_new(const pcr_string *name, pcr_exception ex);

extern pcr_testsuite *
pcr_testsuite_new_2(const pcr_string *name, pcr_unittest **tests, size_t len,
                    pcr_exception ex);

extern pcr_testsuite *
pcr_testsuite_copy(const pcr_testsuite *ctx, pcr_exception ex);

extern size_t
pcr_testsuite_len(const pcr_testsuite *ctx, pcr_exception ex);

extern void
pcr_testsuite_push(pcr_testsuite *ctx, const pcr_testcase *tc,
                        pcr_exception ex);

extern uint64_t
pcr_testsuite_run(pcr_testsuite *ctx, pcr_exception ex);


/******************************************************************************
 * INTERFACE: pcr_testharness
 */

extern void
pcr_testharness_init(const pcr_string *log, pcr_exception ex);

extern void
pcr_testharness_exit(void);

extern void
pcr_testharness_push(const pcr_testsuite *ts, pcr_exception ex);

extern void
pcr_testharness_run(pcr_exception ex);


/******************************************************************************
 * INTERFACE: PCR_ATTRIBUTE
 */

typedef enum PCR_ATTRIBUTE {
    PCR_ATTRIBUTE_NULL,
    PCR_ATTRIBUTE_INT,
    PCR_ATTRIBUTE_FLOAT,
    PCR_ATTRIBUTE_TEXT
} PCR_ATTRIBUTE;


/******************************************************************************
 * INTERFACE: PCR_ATTRIBUTE_VECTOR
 */

typedef pcr_vector PCR_ATTRIBUTE_VECTOR;

inline PCR_ATTRIBUTE_VECTOR *
PCR_ATTRIBUTE_VECTOR_NEW(pcr_exception ex)
{
    return pcr_vector_new(sizeof (PCR_ATTRIBUTE), ex);
}

inline PCR_ATTRIBUTE_VECTOR *
PCR_ATTRIBUTE_VECTOR_NEW_2(const PCR_ATTRIBUTE *arr, size_t len,
                           pcr_exception ex)
{
    pcr_assert_handle(arr, ex);
    pcr_assert_range(len ,ex);

    PCR_ATTRIBUTE_VECTOR *vec = PCR_ATTRIBUTE_VECTOR_NEW(ex);
    for (register size_t i = 0; i < len; i++)
        pcr_vector_push(&vec, &arr[i], ex);

    return vec;
}

inline PCR_ATTRIBUTE_VECTOR *
PCR_ATTRIBUTE_VECTOR_COPY(const PCR_ATTRIBUTE_VECTOR *ctx, pcr_exception ex)
{
    return pcr_vector_copy(ctx, ex);
}

inline size_t
PCR_ATTRIBUTE_VECTOR_LEN(const PCR_ATTRIBUTE_VECTOR *ctx, pcr_exception ex)
{
    return pcr_vector_len(ctx, ex);
}

inline size_t
PCR_ATTRIBUTE_VECTOR_REFCOUNT(const PCR_ATTRIBUTE_VECTOR *ctx, pcr_exception ex)
{
    return pcr_vector_refcount(ctx, ex);
}

inline PCR_ATTRIBUTE
PCR_ATTRIBUTE_VECTOR_ELEM(const PCR_ATTRIBUTE_VECTOR *ctx, size_t idx,
                          pcr_exception ex)
{
    return *((PCR_ATTRIBUTE *) pcr_vector_elem(ctx, idx, ex));
}

inline void
PCR_ATTRIBUTE_VECTOR_ELEM_SET(PCR_ATTRIBUTE_VECTOR **ctx, size_t idx,
                              PCR_ATTRIBUTE elem, pcr_exception ex)
{
    pcr_vector_setelem(ctx, &elem, idx, ex);
}

inline void
PCR_ATTRIBUTE_VECTOR_PUSH(PCR_ATTRIBUTE_VECTOR **ctx, PCR_ATTRIBUTE elem,
                          pcr_exception ex)
{
    pcr_vector_push(ctx, &elem, ex);
}

inline void
PCR_ATTRIBUTE_VECTOR_ITERATE(const PCR_ATTRIBUTE_VECTOR *ctx, pcr_iterator *itr,
                             void *opt, pcr_exception ex)
{
    pcr_vector_iterate(ctx, itr, opt, ex);
}

inline void
PCR_ATTRIBUTE_VECTOR_MUTERATE(PCR_ATTRIBUTE_VECTOR **ctx, pcr_muterator *mtr,
                              void *opt, pcr_exception ex)
{
    pcr_vector_muterate(ctx, mtr, opt, ex);
}

/******************************************************************************
 * INTERFACE: pcr_attribute
 */

typedef struct pcr_attribute pcr_attribute;

extern pcr_attribute *
pcr_attribute_new(PCR_ATTRIBUTE type, const pcr_string *key, const void *value,
                  pcr_exception ex);

inline pcr_attribute *
pcr_attribute_new_null(const pcr_string *key, pcr_exception ex)
{
    return pcr_attribute_new(PCR_ATTRIBUTE_NULL, key, NULL, ex);
}

inline pcr_attribute *
pcr_attribute_new_int(const pcr_string *key, int64_t value, pcr_exception ex)
{
    return pcr_attribute_new(PCR_ATTRIBUTE_INT, key, &value, ex);
}

inline pcr_attribute *
pcr_attribute_new_int_2(const pcr_string *key, pcr_exception ex)
{
    return pcr_attribute_new_int(key, 0, ex);
}

inline pcr_attribute *
pcr_attribute_new_float(const pcr_string *key, double value, pcr_exception ex)
{
    return pcr_attribute_new(PCR_ATTRIBUTE_FLOAT, key, &value, ex);
}

inline pcr_attribute *
pcr_attribute_new_float_2(const pcr_string *key, pcr_exception ex)
{
    return pcr_attribute_new_float(key, 0.0, ex);
}

inline pcr_attribute *
pcr_attribute_new_text(const pcr_string *key, const pcr_string *value,
                       pcr_exception ex)
{
    return pcr_attribute_new(PCR_ATTRIBUTE_TEXT, key, value, ex);
}

inline pcr_attribute *
pcr_attribute_new_text_2(const pcr_string *key, pcr_exception ex)
{
    return pcr_attribute_new_text(key, "", ex);
}

extern pcr_attribute *
pcr_attribute_copy(const pcr_attribute *ctx, pcr_exception ex);

extern PCR_ATTRIBUTE
pcr_attribute_type(const pcr_attribute *ctx, pcr_exception ex);

extern pcr_string *
pcr_attribute_key(const pcr_attribute *ctx, pcr_exception ex);

extern void *
pcr_attribute_value(const pcr_attribute *ctx, pcr_exception ex);

extern size_t
pcr_attribute_valuesz(const pcr_attribute *ctx, pcr_exception ex);

extern pcr_string *
pcr_attribute_string(const pcr_attribute *ctx, pcr_exception ex);

extern pcr_string *
pcr_attribute_json(const pcr_attribute *ctx, pcr_exception ex);


/******************************************************************************
 * INTERFACE: pcr_attribute_vector
 */


typedef pcr_vector pcr_attribute_vector;

inline pcr_attribute_vector *
pcr_attribute_vector_new(pcr_exception ex)
{
    return pcr_vector_new(sizeof (pcr_attribute *), ex);
}

inline pcr_attribute_vector *
pcr_attribute_vector_new_2(const pcr_attribute **arr, size_t len,
                           pcr_exception ex)
{
    pcr_assert_handle(arr, ex);
    pcr_assert_range(len, ex);

    pcr_attribute_vector *vec = pcr_attribute_vector_new(ex);
    for (register size_t i = 0; i < len; i++)
        pcr_vector_push(&vec, &arr[i], ex);

    return vec;
}

inline pcr_attribute_vector *
pcr_attribute_vector_copy(const pcr_attribute_vector *ctx, pcr_exception ex)
{
    return pcr_vector_copy(ctx, ex);
}

inline size_t
pcr_attribute_vector_len(const pcr_attribute_vector *ctx, pcr_exception ex)
{
    return pcr_vector_len(ctx, ex);
}

inline size_t
pcr_attribute_vector_refcount(const pcr_string_vector *ctx, pcr_exception ex)
{
    return pcr_vector_refcount(ctx, ex);
}

inline pcr_attribute *
pcr_attribute_vector_elem(const pcr_attribute_vector *ctx, size_t idx,
                          pcr_exception ex)
{
    return *((pcr_attribute **) pcr_vector_elem(ctx, idx, ex));
}

inline void
pcr_attribute_vector_elem_set(pcr_attribute_vector **ctx, size_t idx,
                              const pcr_attribute *elem, pcr_exception ex)
{
    pcr_vector_setelem(ctx, elem, idx, ex);
}

inline void
pcr_attribute_vector_push(pcr_attribute_vector **ctx, const pcr_attribute *elem,
                          pcr_exception ex)
{
    pcr_vector_push(ctx, &elem, ex);
}

inline void
pcr_attribute_vector_iterate(const pcr_attribute_vector *ctx, pcr_iterator *itr,
                             void *opt, pcr_exception ex)
{
    pcr_vector_iterate(ctx, itr, opt, ex);
}

inline void
pcr_attribute_vector_muterate(pcr_attribute_vector **ctx, pcr_muterator *mtr,
                              void *opt, pcr_exception ex)
{
    pcr_vector_muterate(ctx, mtr, opt, ex);
}


/******************************************************************************
 * INTERFACE: pcr_resultset
 */

typedef struct pcr_resultset pcr_resultset;

extern pcr_resultset *
pcr_resultset_new(const pcr_string *name, const pcr_string_vector *keys,
                  const PCR_ATTRIBUTE_VECTOR *types, pcr_exception ex);

extern pcr_resultset *
pcr_resultset_new_2(const pcr_string *name, const pcr_string **keys,
                    const PCR_ATTRIBUTE *types, size_t len, pcr_exception ex);

extern pcr_resultset *
pcr_resultset_copy(const pcr_resultset *ctx, pcr_exception ex);

extern PCR_ATTRIBUTE_VECTOR *
pcr_resultset_types(const pcr_resultset *ctx, pcr_exception ex);

extern pcr_string_vector *
pcr_resultset_keys(const pcr_resultset *ctx, pcr_exception ex);

extern pcr_vector *
pcr_resultset_values(const pcr_resultset *ctx, pcr_exception ex);

extern size_t
pcr_resultset_rows(const pcr_resultset *ctx, pcr_exception ex);

extern size_t
pcr_resultset_cols(const pcr_resultset *ctx, pcr_exception ex);

extern size_t
pcr_resultset_refcount(const pcr_resultset *ctx, pcr_exception ex);

extern pcr_attribute *
pcr_resultset_attrib(const pcr_resultset *ctx, size_t row, size_t col,
                     pcr_exception ex);

extern void
pcr_resultset_attrib_set(pcr_resultset **ctx, const pcr_attribute *attr,
                         size_t row, size_t col, pcr_exception ex);

extern void
pcr_resultset_push(pcr_resultset **ctx, const pcr_attribute *attr,
                   pcr_exception ex);

extern void
pcr_resultset_push_2(pcr_resultset **ctx, const pcr_attribute_vector *attrs,
                     pcr_exception ex);

extern pcr_string *
pcr_resultset_json(const pcr_resultset *ctx, pcr_exception ex);


/**************************************************************************//**
 * @defgroup pcr_sql PCR SQL Module
 * Manage the preparation of safe SQL statements.
 *
 * SQL statements are the interface to relational databases, and it is often
 * necessary to be able to prepare SQL statements dynamically at runtime. The
 * PCR SQL Module provides this functionality, and also takes into account the
 * sanitisation of SQL statements so that SQL injection attacks are mitigated.
 * @{
 */


/**
 * An SQL statement.
 *
 * The pcr_sql type represents an SQL statement with parameters that may be
 * bound dynamically. This is an abstract data type that can be manipulate only
 * through its interface. The heap memory allocated to pcr_sql instances is
 * managed internally by the PCR Library through the Boehm Garbage Collector.
 * pcr_sql instances are reference counted for performance reasons.
 */
typedef struct pcr_sql pcr_sql;


/**
 * Create new SQL statement.
 *
 * The pcr_sql_new() iterface function creates a new instance of a PCR SQL
 * statement, initialised with its parameterised unbound form @p unbound.
 *
 * @param unbound
 * @param ex The exception stack.
 *
 * @return The new SQL statement.
 *
 * @warning Don't call the standard @c free() function on the SQL statement
 * instance created by this function; the heap memory allocated to the instance
 * is managed internally by the PCR Library through the Boehm Garbage Collector.
 *
 * @see pcr_sql_copy()
 * @see pcr_sql_refcount()
 */
extern pcr_hint_hot pcr_sql *
pcr_sql_new(const pcr_string *unbound, pcr_exception ex);


/**
 * Clone existing SQL statement.
 *
 * The pcr_sql_copy() interface function creates an identical copy of an
 * existing SQL statement instance @p ctx.
 *
 * @param ctx The contextual SQL statement instance.
 * @param ex The exception stack.
 *
 * @return The copy of @p ctx.
 *
 * @note Since pcr_sql instances are reference counted, this function performs a
 * lazy copy.
 *
 * @warning Don't call the standard @c free() function on the SQL statement
 * instance created by this function; the heap memory allocated to the instance
 * is managed internally by the PCR Library through the Boehm Garbage Collector.
 *
 * @see pcr_sql_new()
 * @see pcr_sql_refcount()
 */
extern pcr_hint_hot pcr_sql *
pcr_sql_copy(const pcr_sql *ctx, pcr_exception ex);


/**
 * Get reference count of SQL statement.
 *
 * The pcr_sql_refcount() interface function gets the number of references
 * currently existing in memory of a given SQL statement instance @p ctx.
 *
 * @param ctx The contextual SQL statement instance.
 * @param ex The exception stack.
 *
 * @return The reference count of @p ctx.
 *
 * @note In practice, this function is not required by client code, and has been
 * provided mainly for the purpose of unit testing the PCR SQL Module.
 *
 * @see pcr_sql_new()
 * @see pcr_sql_copy()
 */
extern size_t
pcr_sql_refcount(const pcr_sql *ctx, pcr_exception ex);


/**
 * Get unbound parameterised SQL statement.
 *
 * The pcr_sql_unbound() interface function returns the unbound form of an SQL
 * statement instance in which its SQL parameters have not been provided.
 *
 * @param ctx The contextual SQL statement instance.
 * @param ex The exception stack.
 *
 * @return The unbound parameterised SQL statement.
 *
 * @see pcr_sql_bound()
 */
extern pcr_string *
pcr_sql_unbound(const pcr_sql *ctx, pcr_exception ex);


/**
 * Get bound SQL statement.
 *
 * The pcr_sql_bound() interface function returns the bound form of an SQL
 * statement instance @p ctx in which its SQL parameters have been replaced by
 * their corresponding arguments.
 *
 * @param ctx The contextual SQL statement instance.
 * @param ex The exception stack.
 *
 * @return The bound SQL statement.
 *
 * @see pcr_sql_bind()
 * @see pcr_sql_unbound()
 * @see pcr_sql_reset()
 */
extern pcr_hint_hot pcr_string *
pcr_sql_bound(const pcr_sql *ctx, pcr_exception ex);


/**
 * Bind parameter in SQL statement.
 *
 * The pcr_sql_bind() interface function binds a parameter in an SQL statement
 * @p ctx with its corresponding value. The parameter, its corresponding value,
 * and its type are all specified through the attribute @p attr. This function
 * takes care to ensure that potential SQL injection attacks through text
 * parameter bindings are mitigated.
 *
 * @param ctx The contextual SQL statement instance.
 * @param attr The parameter binding mapping.
 * @param ex The exception stack.
 *
 * @note In practice, it might be more convenient to use the overloaded forms of
 * this function.
 *
 * @see pcr_sql_bind_null()
 * @see pcr_sql_bind_int()
 * @see pcr_sql_bind_float()
 * @see pcr_sql_bind_text()
 */
extern pcr_hint_hot void
pcr_sql_bind(pcr_sql **ctx, const pcr_attribute *attr, pcr_exception ex);


/**
 * Bind null parameter in SQL statement.
 *
 * The pcr_sql_bind_null() interface function is an overloaded version of the
 * pcr_sql_bind() interface function. This function binds the SQL @c NULL value
 * to a parameter @p key in an SQL statement @p ctx.
 *
 * @param ctx The contextual SQL statement instance.
 * @param key The SQL parameter to bind.
 * @param ex The exception stack.
 *
 * @note This function is simply a convenience wrapper around pcr_sql_bind().
 *
 * @see pcr_sql_bind()
 */
inline void
pcr_sql_bind_null(pcr_sql **ctx, const pcr_string *key, pcr_exception ex)
{
    pcr_sql_bind(ctx, pcr_attribute_new_null(key, ex), ex);
}


/**
 * Bind integer parameter in SQL statement.
 *
 * The pcr_sql_bind_int() interface function is an overloaded version of the
 * pcr_sql_bind() interface function. This function binds the integer @p value
 * to a parameter @p key in an SQL statement @p ctx.
 *
 * @param ctx The contextual SQL statement instance.
 * @param key The SQL parameter to bind.
 * @param value The value to bind.
 * @param ex The exception stack.
 *
 * @note This function is simply a convenience wrapper around pcr_sql_bind().
 *
 * @see pcr_sql_bind()
 */
inline void
pcr_sql_bind_int(pcr_sql **ctx, const pcr_string *key, int64_t value,
                 pcr_exception ex)
{
    pcr_sql_bind(ctx, pcr_attribute_new_int(key, value, ex), ex);
}


/**
 * Bind floating point parameter in SQL statement.
 *
 * The pcr_sql_bind_float() interface function is an overloaded version of the
 * pcr_sql_bind() interface function. This function binds the floating point @p
 * value to a parameter @p key in an SQL statement @p ctx.
 *
 * @param ctx The contextual SQL statement instance.
 * @param key The SQL parameter to bind.
 * @param value The value to bind.
 * @param ex The exception stack.
 *
 * @note This function is simply a convenience wrapper around pcr_sql_bind().
 *
 * @see pcr_sql_bind()
 */
inline void
pcr_sql_bind_float(pcr_sql **ctx, const pcr_string *key, double value,
                   pcr_exception ex)
{
    pcr_sql_bind(ctx, pcr_attribute_new_float(key, value, ex), ex);
}


/**
 * Bind text parameter in SQL statement.
 *
 * The pcr_sql_bind_text() interface function is an overloaded version of the
 * pcr_sql_bind() interface function. This function binds the string @p value to
 * a parameter @p key in an SQL statement @p ctx.
 *
 * @param ctx The contextual SQL statement instance.
 * @param key The SQL parameter to bind.
 * @param value The value to bind.
 * @param ex The exception stack.
 *
 * @note This function is simply a convenience wrapper around pcr_sql_bind().
 *
 * @see pcr_sql_bind()
 */
inline void
pcr_sql_bind_text(pcr_sql **ctx, const pcr_string *key, const pcr_string *value,
                  pcr_exception ex)
{
    pcr_sql_bind(ctx, pcr_attribute_new_text(key, value, ex), ex);
}


/**
 * Reset SQL statement.
 *
 * The pcr_sql_reset() interface function resets an SQL statement instance @p
 * ctx back to its unbound state.
 *
 * @param ctx The contextual SQL statement instance.
 * @param ex The exception stack.
 *
 * @note It is safe, albeit pointless, to call this function even if no SQL
 * parameters have been bound in @p ctx.
 */
extern void
pcr_sql_reset(pcr_sql **ctx, pcr_exception ex);


/**
 * @example sql.h
 * This is an example showing how to code against the PCR SQL Module interface.
 * @}
 */


/******************************************************************************
 * INTERFACE: pcr_dbase
 */

typedef struct pcr_dbase pcr_dbase;

typedef enum PCR_DBASE {
    PCR_DBASE_SQLITE
} PCR_DBASE;

extern pcr_dbase *
pcr_dbase_new(PCR_DBASE adapter, const pcr_string *conn, pcr_exception ex);

extern pcr_dbase *
pcr_dbase_copy(const pcr_dbase *ctx, pcr_exception ex);

extern pcr_resultset *
pcr_dbase_query(const pcr_dbase *ctx, const pcr_sql *sql, pcr_exception ex);

extern void
pcr_dbase_command(pcr_dbase *ctx, const pcr_sql *sql, pcr_exception ex);

extern void
pcr_dbase_begin(pcr_dbase *ctx, pcr_exception ex);

extern void
pcr_dbase_commit(pcr_dbase *ctx, pcr_exception ex);

extern void
pcr_dbase_rollback(pcr_dbase *ctx, pcr_exception ex);


/* Lua Script */

typedef struct pcr_lua pcr_lua;

extern pcr_lua *
pcr_lua_open(const pcr_string *path, pcr_exception ex);

extern void
pcr_lua_close(pcr_lua *ctx);

extern bool
pcr_lua_bool(const pcr_lua *lua, const pcr_string *var, pcr_exception ex);

extern int64_t
pcr_lua_int(const pcr_lua *lua, const pcr_string *var, pcr_exception ex);

extern double
pcr_lua_float(const pcr_lua *lua, const pcr_string *var, pcr_exception ex);

/* Evaluates a Lua string variable */
extern pcr_string *
pcr_lua_string(const pcr_lua *lua, const pcr_string *var, pcr_exception ex);

#if defined (__cplusplus)
}
#endif

#endif /* !defined PROTO_C_RUNTIME */

