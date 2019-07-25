#include "../src/api.h"


int compare(int *a, int *b, pcr_exception ex)
{
    if (!(a && b))
        pcr_exception_throw(ex, PCR_EXCEPTION_HANDLE);

    return *a == *b;
}



int do_compare(int *a, int *b, pcr_exception ex)
{
    pcr_exception x;

    pcr_exception_try (x) {
        return compare(a, b, x);
    }

    pcr_exception_unwind(ex);
}


int do_do_compare(int *a, int *b, pcr_exception ex)
{
    auto pcr_exception x;

    pcr_exception_try (x) {
        return do_compare(a, b, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        printf("Bad handle passed...\n");
    }

    pcr_exception_unwind(ex);
}


int main(void)
{
    pcr_exception x;

    pcr_exception_try (x) {
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

