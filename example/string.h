#include <stdio.h>
#include <pcr/api.h>


int main(void)
{
    /* Set up try block. */
    pcr_exception_try (x) {

        /* This is how you could create two strings on the stack. */
        const pcr_string *S1 = "Hello, world!";
        const pcr_string *S2 = "Привет, мир!";

        /* This is now you could create two strings on the heap. */
        pcr_string *s1 = pcr_string_new("Hello, world!", x);
        pcr_string *s2 = pcr_string_new("Привет, мир!", x);

        /* This is how you would make copies of strings. */
        pcr_string *c1 = pcr_string_copy(s1, x);
        pcr_string *c2 = pcr_string_copy("Привет, мир!", x);

        /* This is how would determine the length and size of strings. */
        printf("c2 len = %lu\n", pcr_string_len(c2, x));
        printf("c2 size = %lu\n", pcr_string_sz(c2, x));

        /* This is how you would compare two strings for equality. */
        if (!pcr_string_cmp(s1, S1, x))
            printf("s1 and S1 are equal!\n");

        /* This is how you would check if a string is lexicographically less
         * than another. */
        if (pcr_string_cmp(s2, S2, x) < 1)
            printf("s2 < S2\n");

        /* This is how you would check if a string is lexicographically greater
         * than another. */
        if (pcr_string_cmp(c1, c2, x) > 1)
            printf("c1 > c2\n");

        /* This is how you would add two strings. */
        pcr_string *s3 = pcr_string_add(s1, s2, x);
        s3 = pcr_string_add(s3, s1, x);

        /* This is how you would search for the first instance of a substring in
         * a string. */
        printf("s2 found in s3 at index %lu", pcr_string_find(s3, s2, x));

        /* This is how you would replace the first instance of a substring in a
         * string. */
        pcr_string *s4 = pcr_string_replace_first(s3, s1, "replaced text", x);

        /* This is how you would replace all instances of a substring in a
         * string. */
        s4 = pcr_string_replace(s3, s1, "foo", x);

        /* This is how you would represent an integer as a string. */
        printf("%d as a string is %s", 1024, pcr_string_int(1024, x));

        /* This is how you would stringify a floating point number. */
        printf("%lf as a string is %s", 3.14, pcr_string_float(3.14, x));
    }

    /* Catch all exceptions, logging and displaying them. */
    pcr_exception_catch_all {
        pcr_exception_log();
        pcr_exception_print();
    }

    return 0;
}

