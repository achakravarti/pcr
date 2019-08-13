#include <threads.h>
#include "./api.h"


struct pcr_testcase {
    pcr_unittest *test;
    pcr_string *desc;
};


static thread_local FILE *log_hnd = NULL;


static inline void log_open(const pcr_string *path)
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


#define log_write(m, ...)                     \
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


extern pcr_testcase *pcr_testcase_new(pcr_unittest *test,
                                            const pcr_string *desc,
                                            pcr_exception ex)
{
    pcr_assert_handle(test && desc, ex);

    pcr_exception_try (x) {
        pcr_testcase *tc = pcr_mempool_alloc(sizeof *tc, x);
        tc->test = test;
        tc->desc = pcr_string_copy(desc, x);

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

    pcr_exception_try (x) {
        bool res = ctx->test(x);
        log_write("[%s]: %s\n", res ? "OK" : "**FAIL**", ctx->desc);
        return res;
    }

    pcr_exception_unwind(ex);
    return false;
}


struct pcr_testsuite {
    pcr_vector *tcvec;
    pcr_string *name;
};


extern pcr_testsuite *pcr_testsuite_new(const pcr_string *name, pcr_exception ex)
{
    pcr_assert_handle(name, ex);

    pcr_exception_try (x) {
        pcr_testsuite *ts = pcr_mempool_alloc(sizeof *ts, x);
        ts->tcvec = pcr_vector_new(sizeof (pcr_testcase), x);
        ts->name = pcr_string_copy(name, x);

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
        cpy->tcvec = pcr_vector_copy(ctx->tcvec, x);
        cpy->name = pcr_string_copy(ctx->name, x);

        return cpy;
    }

    pcr_exception_unwind(ex);
    return NULL;
}


extern size_t pcr_testsuite_len(const pcr_testsuite *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        return pcr_vector_len(ctx->tcvec, x);
    }

    pcr_exception_unwind(ex);
    return 0;
}


extern void pcr_testsuite_push(pcr_testsuite *ctx, const pcr_testcase *tc,
                                    pcr_exception ex)
{
    pcr_assert_handle(ctx && tc, ex);

    pcr_exception_try (x) {
        pcr_vector_push(&ctx->tcvec, tc, x);
    }

    pcr_exception_unwind(ex);
}


static void tc_run(void *elem, size_t idx, void *opt, pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_testcase *tc = (pcr_testcase *) elem;
        uint64_t *pass = (uint64_t *) opt;

        log_write("%lu. ", idx);
        *pass += (uint64_t) pcr_testcase_run(tc, x);
    }

    pcr_exception_unwind(ex);
}


extern uint64_t pcr_testsuite_run(pcr_testsuite *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    pcr_exception_try (x) {
        log_border();
        log_write("initialising test suite \'%s\'...\n\n", ctx->name);

        uint64_t pass = 0;
        pcr_vector_muterate(&ctx->tcvec, &tc_run, &pass, x);

        size_t len = pcr_vector_len(ctx->tcvec, x);
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


extern void pcr_testharness_init(const pcr_string *log, pcr_exception ex)
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

