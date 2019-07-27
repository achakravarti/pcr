#if !defined PROTO_C_RUNTIME
#define PROTO_C_RUNTIME

#if defined (__cplusplus)
extern "C" {
#endif


/* compiler hints */

#if (defined __GNUC__ || defined __clang__)
#   define pcr_hint_hot __attribute__((hot))
#   define pcr_hint_cold __attribute__((cold)
#   define pcr_hint_likely(p) (__builtin_expect(!!(p), 1))
#   define pcr_hint_unlikely(p) (__builtin_expect(!!(p), 0))
#else
#   define pcr_hint_hot
#   define pcr_hint_cold
#   define pcr_hint_likely
#   define pcr_hint_unlikely
#   warning pcr: hint macros are disabled on non GCC-compatible compilers
#endif


/* logging */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

extern void pcr_log_open(const char *path, bool flush);
extern void pcr_log_close(void);
extern void pcr_log_write__(const char, const char *, ...);

#define pcr_log_trace(m, ...) \
    pcr_log_write__('T', (m), ##__VA_ARGS__)

#define pcr_log_warning(m, ...) \
    pcr_log_write__('W', (m), ##__VA_ARGS__)

#define pcr_log_error(m, ...) \
    pcr_log_write__('E', (m), ##__VA_ARGS__)



/* exception handling */

#include <setjmp.h>

typedef jmp_buf pcr_exception;
typedef int PCR_EXCEPTION;

#define PCR_EXCEPTION_NONE 0x0
#define PCR_EXCEPTION_HANDLE 0x1
#define PCR_EXCEPTION_STATE 0x2
#define PCR_EXCEPTION_RANGE 0x3
#define PCR_EXCEPTION_MEMPOOL 0x4

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


/* assertion macros */

#define pcr_assert_generic(p, x, id)    \
    if (pcr_hint_unlikely (!(p))) {     \
        pcr_exception_throw((x), (id)); \
    }

#define pcr_assert_handle(p, x) \
    pcr_assert_generic((p), (x), PCR_EXCEPTION_HANDLE)

#define pcr_assert_state(p, x) \
    pcr_assert_generic((p), (x), PCR_EXCEPTION_STATE)

#define pcr_assert_range(p, x) \
    pcr_assert_generic((p), (x), PCR_EXCEPTION_RANGE)


/* memory arena */

extern void *pcr_mempool_alloc(size_t sz, pcr_exception ex);
extern void *pcr_mempool_realloc(void *ptr, size_t sz, pcr_exception ex);


/* unit testing */

typedef bool (pcr_unittest)(void);
typedef struct pcr_testcase pcr_testcase;
typedef struct pcr_testsuite pcr_testsuite;

extern pcr_testcase *pcr_testcase_new(pcr_unittest *test, const char *desc,
                                            pcr_exception ex);
extern pcr_testcase *pcr_testcase_copy(const pcr_testcase *ctx,
                                            pcr_exception ex);
extern bool pcr_testcase_run(pcr_testcase *ctx, pcr_exception ex);

extern pcr_testsuite *pcr_testsuite_new(const char *name, pcr_exception ex);
extern pcr_testsuite *pcr_testsuite_copy(const pcr_testsuite *ctx,
                                                pcr_exception ex);
extern size_t pcr_testsuite_len(const pcr_testsuite *ctx, pcr_exception ex);
extern void pcr_testsuite_push(pcr_testsuite *ctx, const pcr_testcase *tc,
                                    pcr_exception ex);
extern uint64_t pcr_testsuite_run(pcr_testsuite *ctx, pcr_exception ex);

extern void pcr_testharness_init(const char *log, pcr_exception ex);
extern void pcr_testharness_exit(void);
extern void pcr_testharness_push(const pcr_testsuite *ts, pcr_exception ex);
extern void pcr_testharness_run(pcr_exception ex);


/* vector */

typedef struct pcr_vector pcr_vector;
typedef void (pcr_iterator)(const void *elem, size_t idx, void *opt,
                                pcr_exception ex);
typedef void (pcr_muterator)(void *elem, size_t idx, void *opt,
                                pcr_exception ex);
typedef int (pcr_comparator)(const void *ctx, const void *cmp);

extern pcr_vector *pcr_vector_new(size_t elemsz, pcr_exception ex);
extern pcr_vector *pcr_vector_copy(const pcr_vector *ctx, pcr_exception ex);
extern size_t pcr_vector_len(const pcr_vector *ctx, pcr_exception ex);
extern size_t pcr_vector_refcount(const pcr_vector *ctx, pcr_exception ex);
extern void *pcr_vector_elem(const pcr_vector *ctx, size_t idx,
                                    pcr_exception ex);
extern void pcr_vector_setelem(pcr_vector **ctx, const void *elem, size_t idx,
                                    pcr_exception ex);
extern void pcr_vector_sort(pcr_vector **ctx, pcr_comparator *cmp,
                                    pcr_exception ex);
extern void pcr_vector_iterate(const pcr_vector *ctx, pcr_iterator *itr,
                                    void *opt, pcr_exception ex);
extern void pcr_vector_muterate(pcr_vector **ctx, pcr_muterator *mtr, void *opt,
                                        pcr_exception ex);


#if defined (__cplusplus)
}
#endif

#endif /* !defined PROTO_C_RUNTIME */

