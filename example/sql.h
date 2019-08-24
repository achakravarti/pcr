#include <pcr/sql.h>

int main(void)
{
    pcr_exception_try (x) {
        const pcr_string *psql = "SELECT * FROM foo WHERE user <> @user AND"
                                 " attempts > @attempts AND time <= @time AND"
                                 " en = @en AND ru = @ru";

        /* This is how you would create a new SQL statement. */
        pcr_sql *sql = pcr_sql_new(psql, x);

        /* This is how you would bind a null parameter. */
        pcr_sql_bind_null(&sql, "@user", x);

        /* This is how you would bind an integer parameter. */
        pcr_sql_bind_int(&sql, "@attempts", 1024, x);

        /* This is how you would bind a floating point parameter. */
        pcr_sql_bind_float(&sql, "@time", 123.456789, x);

        /* This is how you would bind text parameters. */
        pcr_sql_bind_text(&sql, "@en",  "Hello, world!", x);
        pcr_sql_bind_text(&sql, "@ru", "Привет, мир!", x);

        /* This is how you would copy an SQL statement. */
        pcr_sql *copy = pcr_sql_copy(sql, x);

        /* This is how you would reset an SQL statement. */
        pcr_sql_reset(copy, x);
    }

    pcr_exception_catch_all {
        pcr_exception_log();
        pcr_exception_print();
    }

    return 0;
}

