#include <string.h>
#include "../src/api.h"
#include "./suites.h"


int main(void)
{
    pcr_log_open("test.log", true);

    pcr_exception_try (x) {
        pcr_testsuite *suites[] = {
            pcr_string_testsuite(x), pcr_attribute_testsuite(x),
            pcr_sql_testsuite(x)
        };

        pcr_testharness_init("bld/test.log", x);
        register size_t len = sizeof suites / sizeof *suites;
        for (register size_t i = 0; i < len; i++)
            pcr_testharness_push(suites[i], x);

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

