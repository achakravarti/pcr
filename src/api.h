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


/* exception handling */

#include <setjmp.h>
#include <stdio.h>

typedef jmp_buf pcr_exception;
typedef int PCR_EXCEPTION;

#define PCR_EXCEPTION_NONE 0x0
#define PCR_EXCEPTION_HANDLE 0x1
#define PCR_EXCEPTION_STATE 0x2
#define PCR_EXCEPTION_RANGE 0x3
#define PCR_EXCEPTION_MEMPOOL 0x4

#define pcr_exception_try(x)                          \
    register PCR_EXCEPTION pcr__exid__ = setjmp((x)); \
    if (pcr_hint_likely (!pcr__exid__))

#define pcr_exception_catch(x) \
    if (pcr_hint_unlikely (pcr__exid__ == (x)))

#define pcr_exception_catchall \
    if (pcr_hint_unlikely (pcr__exid__))

#define pcr_exception_throw(x, id) \
    longjmp((x), (id))

#define pcr_exception_trace() \
    printf("Exception 0x%x traced in %s()\n", (unsigned) pcr__exid__, __func__)

#define pcr_exception_unwind(x)    \
    do {                           \
        if (pcr__exid__)           \
            pcr_exception_trace(); \
        longjmp((x), pcr__exid__); \
    } while (0)


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

#include <stdbool.h>
#include <stdint.h>

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


#if defined (__cplusplus)
}
#endif

#endif /* !defined PROTO_C_RUNTIME */

