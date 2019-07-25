#if !defined PROTO_C_RUNTIME
#define PROTO_C_RUNTIME


/* compiler hints */

#if (defined __GNUC__ || defined __clang__)
#   define pcr_hint_hot __attribute__((hot))
#   define pcr_hint_cold __attribute__((cold)
#   define pcr_hint_likely(p) (__builtin_expect(!!(p), 1)
#   define pcr_hint_unlikely(p) (__builtin_expect(!!(p), 0)
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


#endif /* !defined PROTO_C_RUNTIME */

