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


#endif /* !defined PROTO_C_RUNTIME */

