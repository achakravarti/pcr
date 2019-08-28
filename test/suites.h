#include "../src/api.h"

#if !defined PCR_TESTSUITES
#define PCR_TESTSUITES

extern pcr_testsuite *
pcr_string_testsuite(pcr_exception ex);

extern pcr_testsuite *
pcr_attribute_testsuite(pcr_exception ex);

extern pcr_testsuite *
pcr_sql_testsuite(pcr_exception ex);

extern pcr_testsuite *
pcr_resultset_testsuite(pcr_exception ex);

extern pcr_testsuite *
pcr_lua_testsuite(pcr_exception ex);

#endif /* !defined PCR_TESTSUITES */

