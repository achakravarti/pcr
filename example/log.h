#include <pcr/api.h>


int main(void)
{
    /* This is how you would open a log file, flushing out its existing
     * contents. */
    const char *path = "/path/to/log/file";
    pcr_log_open(path, true);

    /* This is how you would write a formatted trace message to the opened log
     * file. */
    pcr_log_trace("There are %d days in %d year.\n", 7, 1);

    /* This is how you would write an unformatted warning message to the opened
     * log file. */
    pcr_log_warning("Warning! You need to be careful!");

    /* This is how you would temporarily disable logging of messages. */
    pcr_log_suppress();

    /* This has no effect because logging is now suppressed. */
    pcr_log_warning("Warning message #%d", 2);

    /* This is how you would toggle back on the logging of messages. */
    pcr_log_allow();

    /* This trace message will now be written since logging is now allowed. */
    pcr_log_trace("Logging has been re-enabled.");

    /* Calling pcr_log_open() again closes the currently open log file and opens
     * a new one, setting the latter as the default. In this case, the existing
     * contents of the new log file are preserved. */
    pcr_log_open("/another/log/file", false);

    /* This is how you would write a formatted error message to the currently
     * open log file. */
    pcr_log_error("Exception 0x%x caught in %s\n", 0xDEADBEEF, __func__);

    /* This is how you would close the currently open log file after you are
     * done. */
    pcr_log_close();
}

