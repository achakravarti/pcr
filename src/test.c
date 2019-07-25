#include <string.h>
#include "./api.h"


struct pcr_testcase {
    pcr_unittest *test;
    char *desc;
};


extern pcr_testcase *pcr_testcase_new(pcr_unittest *test, const char *desc,
                                            pcr_exception ex)
{
    pcr_assert_handle(test && desc, ex);

    pcr_exception x;
    pcr_exception_try (x) {
        pcr_testcase *tc = pcr_mempool_alloc(sizeof *tc, x);

        const size_t len = strlen(desc);
        tc->test = test;
        tc->desc = pcr_mempool_alloc(len + 1, x);

        strncpy(tc->desc, desc, len);
        tc->desc[len] = '\0';

        return tc;
    }

    pcr_exception_unwind(ex);
}


extern bool pcr_testcase_run(pcr_testcase *ctx, pcr_exception ex)
{
    pcr_assert_handle(ctx, ex);

    return ctx->test();
}


struct pcr_testsuite {
    pcr_testcase **tests;
    char *name;
    size_t cap;
    size_t len;
};


extern pcr_testsuite *pcr_testsuite_new(const char *name, pcr_exception ex)
{
    pcr_assert_handle(name, ex);

    pcr_exception x;
    pcr_exception_try (x) {
        pcr_testsuite *ts = pcr_mempool_alloc(sizeof *ts, x);

        ts->tests = pcr_mempool_alloc(sizeof *ts->tests, x);
        ts->len = 0;
        ts->cap = 1;

        const size_t slen = strlen(name);
        ts->name = pcr_mempool_alloc(slen + 1, x);
        strncpy(ts->name, name, slen);
        ts->name[slen] = '\0';
    }

    pcr_exception_unwind(ex);
}


extern uint64_t pcr_testsuite_len(const pcr_testsuite *ctx, pcr_exception ex)
{
}


extern void pcr_testsuite_push(pcr_testsuite *ctx, const pcr_testcase *tc,
                                    pcr_exception ex)
{
}


extern uint64_t pcr_testsuite_run(pcr_testsuite *ctx)
{
}

