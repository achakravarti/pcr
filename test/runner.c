#include <string.h>
#include "../src/api.h"


int compare(int *a, int *b, pcr_exception ex)
{
    pcr_assert_state(a && b, ex);
    return *a == *b;
}



int do_compare(int *a, int *b, pcr_exception ex)
{
    pcr_exception_try (x) {
        return compare(a, b, x);
    }

    pcr_exception_unwind(ex);
}


int do_do_compare(int *a, int *b, pcr_exception ex)
{
    pcr_exception_try (x) {
        return do_compare(a, b, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        printf("Bad handle passed...\n");
    }

    pcr_exception_unwind(ex);
}


char *make_str(const char *str, pcr_exception ex)
{
    pcr_exception_try (x) {
        register size_t len = strlen(str);
        char *bfr = pcr_mempool_alloc(len + 1, x);
        strncpy(bfr, str, len);
        bfr[len] = '\0';

        return bfr;
    }

    pcr_exception_unwind(ex);
}


static bool test_dummy(void)
{
    pcr_exception_try (x) {
        int a = 5, b = 6;
        compare(0, &b, x);

        return false;
    }

    pcr_exception_catch (PCR_EXCEPTION_STATE) {
        return true;
    }
}


int main(void)
{
    pcr_exception_try (x) {
        char *bfr = make_str("Hello, world! Goodbye, world!", x);
        printf("bfr = %s\n", bfr);

        pcr_testsuite *ts = pcr_testsuite_new("Rough Tests", x);
        pcr_testcase *tc = pcr_testcase_new(&test_dummy, "Dummy Test", x);
        pcr_testsuite_push(ts, tc, x);
        pcr_testsuite_run(ts, x);

        int a = 5;
        int b = 6;
        printf("a == b: %d\n", do_do_compare(&a, &b, x));
        printf("a == b: %d\n", do_do_compare(0, &b, x));
    }

    pcr_exception_catchall {
        pcr_exception_trace();
    }

    return 0;
}

