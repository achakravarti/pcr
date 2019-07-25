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
        tc->test = test;

        const size_t len = strlen(desc);
        tc->desc = pcr_mempool_alloc(len + 1, x);
        strncpy(tc->desc, desc, len);
        tc->desc[len] = '\0';

        return tc;
    }

    pcr_exception_unwind(ex);
}


extern bool pcr_testcase_run(pcr_testcase *ctx, pcr_exception ex)
{
}

