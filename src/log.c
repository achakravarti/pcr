#include <threads.h>
#include "./api.h"


static thread_local FILE *log_file = NULL;


extern void pcr_log_open(const char *path, bool flush)
{
    if (pcr_hint_unlikely (log_file))
        (void) fclose(log_file);

    const char *mode = flush ? "w" : "a+";
    if (pcr_hint_unlikely (!(path && *path && (log_file = fopen(path, mode)))))
        printf("[warning] pcr_log_open(): cannot open log file %s\n", path);
}


extern void pcr_log_close(void)
{
    if (pcr_hint_likely (log_file)) {
        (void) fclose(log_file);
        log_file = NULL;
    }
}


extern void pcr_log_write__(const char type, const char *func, const char *file,
                                    uint64_t line, const char *msg)
{

    time_t tm = time(NULL);
    char *ctm = ctime(&tm);

    const char *fmt = "[%c] [%.24s] [%s():%s:%d] %s\n";
    if (pcr_hint_likely (log_file && msg && *msg)) {
        (void) fprintf(log_file, fmt, type, ctm, func, file, line, msg);
    }
}

