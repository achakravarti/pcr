#include <gc.h>
#include "api.h"


extern void *pcr_mempool_alloc(size_t sz, pcr_exception ex)
{
    pcr_assert_range(sz, ex);

    auto void *bfr = GC_MALLOC(sz);
    if (pcr_hint_unlikely (!bfr))
        pcr_exception_throw(ex, PCR_EXCEPTION_MEMPOOL);

    return bfr;
}


extern void *pcr_mempool_realloc(void *ptr, size_t sz, pcr_exception ex)
{
}

