#include <threads.h>
#include <stdarg.h>
#include "./api.h"


/* Declare the handle to the file where the log messages will be written. This
 * global handle needs to have thread local storage as calls to the pcr_log
 * interface may originate from separate threads. */
static thread_local FILE *log_file = NULL;


/* Declare the flag that indicates whether or not logging is currently
 * suppressed. Just as in the case of the log_file handle declared above, this
 * global variable too needs to have thread local storage. */
static thread_local bool log_enabled = true;


/* Implement the pcr_log_open() interface function. We use the @flush argument
 * to determine the mode in which the log file is to be opened.
 *
 * There are two possible unlikely edge cases:
 *   - the log file is already open, and
 *   - the log file fails to open.
 *
 * In the former case, the log file that is already open is first closed, and in
 * the latter case, a warning message is printed onto stdout.
 */
extern void pcr_log_open(const char *path, bool flush)
{
    if (pcr_hint_unlikely (log_file))
        (void) fclose(log_file);

    const char *mode = flush ? "w" : "a+";
    if (pcr_hint_unlikely (!(path && *path && (log_file = fopen(path, mode)))))
        printf("[warning] pcr_log_open(): cannot open log file %s\n", path);
}


/* Implement the pcr_log_close() interface function. */
extern void pcr_log_close(void)
{
    if (pcr_hint_likely (log_file)) {
        (void) fclose(log_file);
        log_file = NULL;
    }
}


/* Implement the pcr_log_allow() interface function. */
extern void
pcr_log_allow(void)
{
    log_enabled = true;
}


/* Implement the pcr_log_suppress() interface function */
extern void
pcr_log_suppress(void)
{
    log_enabled = false;
}


/* Implement the private helper function pcr_log_write__(). This function
 * performs the core process of writing timestamped log messages, and is called
 * by the interface macros.
 *
 * This function writes log messages in the following format:
 *   [@type] [timestamp]: @msg
 *
 * A concrete example of a trace message written by this function is as follows:
 *   [T] [Tue Aug 20 12:37:52 2019]: started PCR Library test runner...
 *
 * The argument to @type can be one of T, W, or E for trace, warning, or error
 * messages, respectively. The formatted message is passed through @msg, and the
 * format tokens are passed as a variable argument list. No error checking is
 * done on the arguments as this function is private and not called directly by
 * client code.
 */
extern void pcr_log_write__(const char type, const char *msg, ...)
{
    if (pcr_hint_unlikely (!log_enabled))
        return;

    time_t tm = time(NULL);
    char *ctm = ctime(&tm);

    (void) fprintf(log_file, "[%c] [%.24s]: ", type, ctm);

    auto va_list va;
    va_start(va, msg);
    (void) vfprintf(log_file, msg, va);
    va_end(va);

    (void) fputs("\n", log_file);
}

