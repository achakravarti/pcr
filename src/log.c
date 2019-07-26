#include <threads.h>
#include "./api.h"


static thread_local FILE *log_file = NULL;


extern void pcr_log_open(const char *path, bool flush)
{
    const char *mode = flush ? "w" : "a+";
    if (pcr_hint_unlikely (!(path && (log_file = fopen(path, mode)))))
        printf("[warning] pcr_log_open(): cannot open log file %s\n", path);
}


extern void pcr_log_close(void)
{
}


extern void pcr_log_write__(const char type, const char *func, const char *file,
                                    uint64_t line, const char *msg)
{
}

