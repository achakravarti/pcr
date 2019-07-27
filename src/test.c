#include <threads.h>
#include <string.h>
#include "./api.h"


struct pcr_testcase {
    pcr_unittest *test;
    char *desc;
};


static thread_local FILE *log_hnd = NULL;


static inline void log_open(const char *path)
{
    if (pcr_hint_unlikely (log_hnd))
        (void) fclose(log_hnd);

    if (pcr_hint_unlikely (!(path && *path && (log_hnd = fopen(path, "w")))))
        printf("[warning] pcr_testlog_open(): cannot open test log %s\n", path);
}


static inline void log_close(void)
{
    if (pcr_hint_likely (log_hnd)) {
        (void) fclose(log_hnd);
        log_hnd = NULL;
    }
}


#define log_write(m, ...)                      \
    if (pcr_hint_likely (log_hnd)) {          \
        fprintf(log_hnd, (m), ##__VA_ARGS__); \
    }


#define log_tee(m, ...)                \
    do {                               \
        log_write((m), ##__VA_ARGS__); \
        printf(m, ##__VA_ARGS__);      \
    } while (0)


static void log_border(void)
{
    if (pcr_hint_likely (log_hnd)) {
        fputs("\n", log_hnd);

        for (register size_t i = 0; i < 80; i++)
            fputs("=", log_hnd);

        fputs("\n", log_hnd);
    }
}


extern pcr_testcase *pcr_testcase_new(pcr_unittest *test, const char *desc,
                                            pcr_exception ex)
{
    pcr_assert_handle(test && desc, ex);

    pcr_exception_try (x) {
        pcr_testcase *tc = pcr_mempool_alloc(sizeof *tc, x);
        tc->test = test;

        const size_t len = strlen(desc) + 1;
        tc->desc = pcr_mempool_alloc(len, x);
        (void) strncpy(tc->desc, desc, len);

        return tc;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_testcase *pcr_testcase_copy(const pcr_testcase *ctx,
                                            pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        return pcr_testcase_new(ctx->test, ctx->desc, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern bool pcr_testcase_run(pcr_testcase *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    bool res = ctx->test();
    log_write("[%s]: %s\n", res ? "OK" : "**FAIL**", ctx->desc);

    return res;
}


struct pcr_testsuite {
    pcr_testcase **tests;
    char *name;
    size_t cap;
    size_t len;
};


static void vec_resize(pcr_testsuite *ctx, pcr_exception ex)
{
    pcr_exception_try (x) {
        ctx->cap *= 2;
        ctx->tests = pcr_mempool_realloc(
                            ctx->tests, sizeof *ctx->tests * ctx->cap, x);
    }

    pcr_exception_unwind(ex);
}


extern pcr_testsuite *pcr_testsuite_new(const char *name, pcr_exception ex)
{
    pcr_assert_handle(name, ex);

    pcr_exception_try (x) {
        pcr_testsuite *ts = pcr_mempool_alloc(sizeof *ts, x);

        ts->len = 0;
        ts->cap = 4;
        ts->tests = pcr_mempool_alloc(sizeof *ts->tests * ts->cap, x);

        const size_t slen = strlen(name) + 1;
        ts->name = pcr_mempool_alloc(slen, x);
        (void) strncpy(ts->name, name, slen);

        return ts;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern pcr_testsuite *pcr_testsuite_copy(const pcr_testsuite *ctx,
                                            pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        pcr_testsuite *cpy = pcr_mempool_alloc(sizeof *cpy, x);
        cpy->len = ctx->len;
        cpy->cap = ctx->cap;

        size_t sz = sizeof *cpy->tests * cpy->cap;
        cpy->tests = pcr_mempool_alloc(sz, x);
        memcpy(cpy->tests, ctx->tests, sz);

        sz = strlen(ctx->name) + 1;
        cpy->name = pcr_mempool_alloc(sz, x);
        strncpy(cpy->name, ctx->name, sz);

        return cpy;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern size_t pcr_testsuite_len(const pcr_testsuite *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    return ctx->len;
}


extern void pcr_testsuite_push(pcr_testsuite *ctx, const pcr_testcase *tc,
                                    pcr_exception ex)
{
    pcr_assert_handle(ctx && tc, ex);

    pcr_exception_try (x) {
        if (pcr_hint_unlikely (ctx->len == ctx->cap))
            vec_resize(ctx, x);

        ctx->tests[ctx->len++] = pcr_testcase_copy(tc, x);
    }

    pcr_exception_unwind(ex);
}


extern uint64_t pcr_testsuite_run(pcr_testsuite *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        log_border();
        log_write("initialising test suite \'%s\'...\n\n", ctx->name);

        register uint64_t pass = 0, len = ctx->len;
        for (register uint64_t i = 0; i < len; i++) {
            log_write("%lu. ", i + 1);
            pass += (uint64_t) pcr_testcase_run(ctx->tests[i], x);
        }

        log_write("\ncompleted running test suite \'%s\'...\n", ctx->name);
        log_write("%lu passed, %lu failed, %lu total", pass, len - pass, len);
        log_border();

        return pass;
    }

    pcr_exception_unwind(ex);
    return 0;
}


struct {
    pcr_vector *tsvec;
    uint64_t pass;
    uint64_t total;
} *th_hnd;


extern void pcr_testharness_init(const char *log, pcr_exception ex)
{
    pcr_exception_try (x) {
        log_open(log);

        th_hnd = pcr_mempool_alloc(sizeof *th_hnd, x);
        th_hnd->tsvec = pcr_vector_new(sizeof (pcr_testsuite), x);
        th_hnd->pass = th_hnd->total = 0;
    }

    pcr_exception_unwind(ex);
}


extern void pcr_testharness_exit(void)
{
    log_close();
}


extern void pcr_testharness_push(const pcr_testsuite *ts, pcr_exception ex)
{
    pcr_assert_state(th_hnd, ex);
    pcr_assert_handle(ts, ex);

    pcr_exception_try (x) {
        pcr_vector_push(&th_hnd->tsvec, (void *) ts, x);
    }

    pcr_exception_unwind(ex);
}


static void ts_run(void *elem, size_t idx, void *opt, pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_testsuite *ts = (pcr_testsuite *) elem;
        th_hnd->pass += pcr_testsuite_run(ts, x);
        th_hnd->total += pcr_testsuite_len(ts, x);
    }

    pcr_exception_unwind(ex);
}


extern void pcr_testharness_run(pcr_exception ex)
{
    pcr_assert_state(th_hnd, ex);

    pcr_exception_try (x) {
        pcr_vector_muterate(&th_hnd->tsvec, &ts_run, NULL, x);

        log_tee("\ncompleted running %lu test suite(s)...\n",
                        pcr_vector_len(th_hnd->tsvec, x));
        log_tee("%lu passed, %lu failed, %lu total\n", th_hnd->pass,
                        th_hnd->total - th_hnd->pass, th_hnd->total);
    }

    pcr_exception_unwind(ex);
}

