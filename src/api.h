#if !defined PROTO_C_RUNTIME
#define PROTO_C_RUNTIME

#if defined (__cplusplus)
extern "C" {
#endif


/******************************************************************************
 * INTERFACE: pcr_hint
 */

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


/******************************************************************************
 * INTERFACE: pcr_log
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

extern void
pcr_log_open(const char *path, bool flush);

extern void
pcr_log_close(void);

extern void
pcr_log_write__(const char, const char *, ...);

#define pcr_log_trace(m, ...) \
    pcr_log_write__('T', (m), ##__VA_ARGS__)

#define pcr_log_warning(m, ...) \
    pcr_log_write__('W', (m), ##__VA_ARGS__)

#define pcr_log_error(m, ...) \
    pcr_log_write__('E', (m), ##__VA_ARGS__)


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


/******************************************************************************
 * INTERFACE: pcr_string
 */

typedef char pcr_string;
typedef pcr_vector pcr_string_vector;

extern pcr_string *
pcr_string_new(const char *cstr, pcr_exception ex);

extern pcr_string *
pcr_string_copy(const pcr_string *ctx, pcr_exception ex);

extern size_t
pcr_string_len(const pcr_string *ctx, pcr_exception ex);

extern pcr_string *
pcr_string_add(const pcr_string *ctx, const pcr_string *add, pcr_exception ex);

extern size_t
pcr_string_find(const pcr_string *haystack, const pcr_string *needle,
                    pcr_exception ex);

extern pcr_string *
pcr_string_replace(const pcr_string *haystack, const pcr_string *needle,
                        const pcr_string *replace, pcr_exception ex);

extern pcr_string *
pcr_string_replaceall(const pcr_string *haystack, const pcr_string *needle,
                            const pcr_string *replace, pcr_exception ex);

extern pcr_string *
pcr_string_parseint(int64_t value, pcr_exception ex);

extern pcr_string *
pcr_string_parsefloat(double value, pcr_exception ex);


/******************************************************************************
 * INTERFACE: pcr_testcase
 */

typedef bool
(pcr_unittest)(void);

typedef struct pcr_testcase pcr_testcase;

extern pcr_testcase *
pcr_testcase_new(pcr_unittest *test, const pcr_string *desc, pcr_exception ex);

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
 * INTERFACE: pcr_field
 */

typedef struct pcr_field pcr_field;

typedef enum PCR_FIELD {
    PCR_FIELD_NULL,
    PCR_FIELD_INT,
    PCR_FIELD_FLOAT,
    PCR_FIELD_TEXT
} PCR_FIELD;

typedef pcr_vector PCR_FIELD_VECTOR;
typedef pcr_vector pcr_field_vector;

extern pcr_field *
pcr_field_new(PCR_FIELD type, size_t elemsz, const pcr_string *key,
                    const void *value, pcr_exception ex);

extern pcr_field *
pcr_field_copy(const pcr_field *ctx, pcr_exception ex);

extern PCR_FIELD
pcr_field_type(const pcr_field *ctx, pcr_exception ex);

extern pcr_string *
pcr_field_key(const pcr_field *ctx, pcr_exception ex);

extern size_t
pcr_field_refcount(const pcr_field *ctx, pcr_exception ex);

extern void *
pcr_field_value(const pcr_field *ctx, pcr_exception ex);

extern void
pcr_field_setvalue(pcr_field **ctx, const void *value, pcr_exception ex);

extern pcr_string *
pcr_field_string(const pcr_field *ctx, pcr_exception ex);

extern pcr_string *
pcr_field_json(const pcr_field *ctx, pcr_exception ex);


/******************************************************************************
 * INTERFACE: pcr_resultset
 */

struct pcr_resultset {
    pcr_string *name;
    pcr_string_vector *keys;
    PCR_FIELD_VECTOR *types;
    pcr_vector *values;
    size_t ref;
};

typedef struct pcr_resultset pcr_resultset;

extern pcr_resultset *
pcr_resultset_new(const pcr_string *name, const pcr_string_vector *keys,
                  const PCR_FIELD_VECTOR *types, pcr_exception ex);

extern pcr_resultset *
pcr_resultset_copy(const pcr_resultset *ctx, pcr_exception ex);

extern pcr_string_vector *
pcr_resultset_keys(const pcr_resultset *ctx, pcr_exception ex);

extern PCR_FIELD_VECTOR *
pcr_resultset_types(const pcr_resultset *ctx, pcr_exception ex);

extern size_t
pcr_resultset_rows(const pcr_resultset *ctx, pcr_exception ex);

extern size_t
pcr_resultset_cols(const pcr_resultset *ctx, pcr_exception ex);

extern pcr_field *
pcr_resultset_field(const pcr_resultset *ctx, size_t row, size_t col,
                    pcr_exception ex);

extern void
pcr_resultset_setfield(pcr_resultset **ctx, const pcr_field *field, size_t row,
                       size_t col, pcr_exception ex);

extern void
pcr_resultset_push(pcr_resultset **ctx, const pcr_field *field,
                   pcr_exception ex);

extern pcr_string *
pcr_resultset_json(const pcr_resultset *ctx, pcr_exception ex);


/******************************************************************************
 * INTERFACE: pcr_sql
 */

typedef struct pcr_sql pcr_sql;

extern pcr_sql *
pcr_sql_new(const pcr_string *unbound, pcr_exception ex);

extern pcr_sql *
pcr_sql_copy(const pcr_sql *ctx, pcr_exception ex);

extern pcr_string *
pcr_sql_unbound(const pcr_sql *ctx, pcr_exception ex);

extern pcr_string *
pcr_sql_bound(const pcr_sql *ctx, pcr_exception ex);

extern void
pcr_sql_bind(pcr_sql **ctx, const pcr_field *field, pcr_exception ex);


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

extern pcr_record_vector *
pcr_dbase_query(const pcr_dbase *ctx, const pcr_sql *sql, pcr_exception ex);

extern void
pcr_dbase_command(pcr_dbase *ctx, const pcr_sql *sql, pcr_exception ex);

extern void
pcr_dbase_begin(pcr_dbase *ctx, pcr_exception ex);

extern void
pcr_dbase_commit(pcr_dbase *ctx, pcr_exception ex);

extern void
pcr_dbase_rollback(pcr_dbase *ctx, pcr_exception ex);


#if defined (__cplusplus)
}
#endif

#endif /* !defined PROTO_C_RUNTIME */

