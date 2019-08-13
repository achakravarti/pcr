#include <string.h>
#include "../src/api.h"
#include "./suites.h"


int main(void)
{
    pcr_log_open("test.log", true);

    pcr_exception_try (x) {
        pcr_testharness_init("bld/test.log", x);
        pcr_testharness_push(pcr_string_testsuite(x), x);
        pcr_testharness_run(x);
        pcr_testharness_exit();
    }

    pcr_exception_catchall {
        pcr_exception_log();
        pcr_exception_print();
    }

    pcr_log_close();
    return 0;
}

