#include <string.h>
#include "./suites.h"


/******************************************************************************
 * pcr_string_new() test cases
 */


static bool
new_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_new() can create an empty string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        return test && !*test;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
new_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_new() can create an ASCII string";

    pcr_exception_try (x) {
        const char *expect = "Hello, world!";
        pcr_string *test = pcr_string_new(expect, x);

        return test && !strcmp(test, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
new_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_new() can create a Unicode string";

    pcr_exception_try (x) {
        const char *expect = "Привет, мир!";
        pcr_string *test = pcr_string_new(expect, x);

        return test && !strcmp(test, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
new_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_new() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @cstr";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_string_new(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_copy() test cases
 */


static bool
copy_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_copy() can copy an empty string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        pcr_string *copy = pcr_string_copy(test, x);

        return copy && !*copy;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
copy_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_copy() can copy an ASCII string";

    pcr_exception_try (x) {
        const char *expect = "Hello, world!";
        pcr_string *test = pcr_string_new(expect, x);
        pcr_string *copy = pcr_string_copy(test, x);

        return test && !strcmp(copy, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
copy_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_copy() can copy a Unicode string";

    pcr_exception_try (x) {
        const char *expect = "Привет, мир!";
        pcr_string *test = pcr_string_new(expect, x);
        pcr_string *copy = pcr_string_copy(test, x);

        return test && !strcmp(copy, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
copy_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_copy() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @ctx";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_string_copy(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_len() test cases
 */


static bool
len_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_len() reports 0 for a null string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        return pcr_string_len(test, x) == 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
len_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_len() reports the length of an ASCII string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        return pcr_string_len(test, x) == 13;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
len_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_len() reports the length of a Unicode string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        return pcr_string_len(test, x) == 12;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
len_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_len() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @ctx";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_string_len(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_sz() test cases
 */


static bool
sz_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_sz() reports 1 for a null string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        return pcr_string_sz(test, x) == 1;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
sz_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_sz() reports the size of an ASCII string";

    pcr_exception_try (x) {
        const pcr_string *sample = "Hello, world!";
        pcr_string *test = pcr_string_new(sample, x);

        return pcr_string_sz(test, x) == strlen(sample) + 1;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
sz_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_sz() reports the size of a Unicode string";

    pcr_exception_try (x) {
        const pcr_string *sample = "Привет, мир!";

        pcr_string *test = pcr_string_new(sample, x);
        return pcr_string_sz(test, x) == strlen(sample) + 1;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
sz_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_sz() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @ctx";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_string_sz(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_cmp() test cases
 */


static bool
cmp_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_cmp() detects two equal null strings";

    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("", x);
        pcr_string *rhs = pcr_string_new("", x);

        return !pcr_string_cmp(lhs, rhs, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
cmp_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_cmp() detects two equal ASCII strings";

    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("Hello, world!", x);
        pcr_string *rhs = pcr_string_new("Hello, world!", x);

        return !pcr_string_cmp(lhs, rhs, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
cmp_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_cmp() detects two equal Unicode strings";

    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("Привет, мир!", x);
        pcr_string *rhs = pcr_string_new("Привет, мир!", x);

        return !pcr_string_cmp(lhs, rhs, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
cmp_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_cmp() detects two unequal ASCII strings";

    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("Hello, world!", x);
        pcr_string *rhs = pcr_string_new("Goodbye, moon?", x);

        return pcr_string_cmp(lhs, rhs, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
cmp_test_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_cmp() detects two unequal Unicode strings";

    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("Привет, мир!", x);
        pcr_string *rhs = pcr_string_new("До свидания, луна?", x);

        return pcr_string_cmp(lhs, rhs, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
cmp_test_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_cmp() detects a lexicographically smaller ASCII string";

    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("Goodbye, moon?", x);
        pcr_string *rhs = pcr_string_new("Hello, world!", x);

        return pcr_string_cmp(lhs, rhs, x) < 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
cmp_test_7(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_cmp() detects a lexicographically smaller Unicode"
            " string";

    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("До свидания, луна?", x);
        pcr_string *rhs = pcr_string_new("Привет, мир!", x);

        return pcr_string_cmp(lhs, rhs, x) < 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
cmp_test_8(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_cmp() detects a lexicographically greater ASCII string";

    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("Goodbye, moon?", x);
        pcr_string *rhs = pcr_string_new("Hello, world!", x);

        return pcr_string_cmp(rhs, lhs, x) > 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
cmp_test_9(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_cmp() detects a lexicographically greater Unicode"
            " string";

    pcr_exception_try (x) {
        pcr_string *lhs = pcr_string_new("До свидания, луна?", x);
        pcr_string *rhs = pcr_string_new("Привет, мир!", x);

        return pcr_string_cmp(rhs, lhs, x) > 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
cmp_test_10(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_cmp() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @lhs";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_string_cmp(NULL, "Hello, world!", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
cmp_test_11(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_cmp() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @rhs";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_string_cmp("Hello, world!", NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_add() test cases
 */


static bool
add_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_add() adds two null strings";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        test = pcr_string_add(test, "", x);

        return !*test;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
add_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_add() adds two ASCII strings";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello", x);
        test = pcr_string_add(test, ", ", x);
        test = pcr_string_add(test, "world", x);
        test = pcr_string_add(test, "!", x);

        return !strcmp(test, "Hello, world!");
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
add_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_add() adds two Unicode strings";

    pcr_exception_try (x) {
        const char *expect = "До свидания, луна?";

        pcr_string *test = pcr_string_new("До свидания", x);
        test = pcr_string_add(test, ", ", x);
        test = pcr_string_add(test, "луна", x);
        test = pcr_string_add(test, "?", x);

        return !strcmp(test, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
add_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_add() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @ctx";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello", x);
        (void) test;

        pcr_log_suppress();
        test = pcr_string_add(NULL, ", ", x);
        (void) test;
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
add_test_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_add() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @add";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello", x);

        pcr_log_suppress();
        test = pcr_string_add(test, NULL, x);
        (void) test;
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_find() test cases
 */


static bool
find_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can find a null string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        return pcr_string_find(test, "", x) == 1;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
find_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can find an ASCII string with 1 character";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        return pcr_string_find(test, "w", x) == 8;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
find_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can find a Unicode string with 1 character";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        return pcr_string_find(test, "м", x) == 9;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
find_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can find an ASCII string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        return pcr_string_find(test, "world", x) == 8;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
find_test_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can find a Unicode string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        return pcr_string_find(test, "мир", x) == 9;
    }

    pcr_exception_unwind(ex);
    return false;
}



static bool
find_test_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can detect the absence of an ASCII string with 1"
            " character";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        return pcr_string_find(test, "h", x) == 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
find_test_7(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can detect the absence of a Unicode string with"
            " 1 character";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        return pcr_string_find(test, "л", x) == 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
find_test_8(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can detect the absence of an ASCII string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        return pcr_string_find(test, "moon", x) == 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
find_test_9(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can detect the absence of a Unicode string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        return pcr_string_find(test, "луна", x) == 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
find_test_10(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @haystack";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_string_find(NULL, "Hello", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
find_test_11(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @needle";

    pcr_exception_try (x) {
        pcr_log_suppress();
        (void) pcr_string_find("Hello", NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_replace() test cases
 */


static bool
replace_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() throws PCR_EXCEPTION_STRING if passed a null"
            " string for @needle";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);

        pcr_log_suppress();
        (void) pcr_string_replace(test, "", "Hello, world!", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replace_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() replaces an ASCII character with a null"
            " character";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        test = pcr_string_replace(test, "!", "", x);

        return !strcmp(test, "Hello, world");
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replace_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() replaces a Unicode character with a null"
            " character";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        test = pcr_string_replace(test, "т", "", x);

        return !strcmp(test, "Приве, мир!");
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replace_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() replaces only the first instance in an ASCII"
            " string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        test = pcr_string_replace(test, "l", "y", x);

        return !strcmp(test, "Heylo, world!");
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replace_test_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() replaces only the first instance in a Unicode"
            " string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        test = pcr_string_replace(test, "р", "r", x);

        return !strcmp(test, "Пrивет, мир!");
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replace_test_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() replaces an ASCII substring";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        test = pcr_string_replace(test, "world", "moon", x);

        return !strcmp(test, "Hello, moon!");
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replace_test_7(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() replaces a Unicode substring";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        test = pcr_string_replace(test, "Привет", "До свидания", x);

        return !strcmp(test, "До свидания, мир!");
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replace_test_8(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() replaces an entire ASCII string";

    pcr_exception_try (x) {
        const pcr_string *expect = "Goodbye, moon?";
        pcr_string *test = pcr_string_new("Hello, world!", x);
        test = pcr_string_replace(test, "Hello, world!", expect, x);

        return !strcmp(test, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replace_test_9(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() replaces an entire Unicode string";

    pcr_exception_try (x) {
        const pcr_string *expect = "До свидания, луна?";
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        test = pcr_string_replace(test, "Привет, мир!", expect, x);

        return !strcmp(test, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replace_test_10(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @haystack";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);

        pcr_log_suppress();
        (void) pcr_string_replace(NULL, test, "", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replace_test_11(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() returns @haystack if @haystack, @needle, and "
            " @replace are all the same";

    pcr_exception_try (x) {
        const pcr_string *h = pcr_string_new("Hello, world!", x);
        const pcr_string *n = pcr_string_new("Hello, world!", x);
        const pcr_string *r = pcr_string_new("Hello, world!", x);

        return !strcmp(h, pcr_string_replace(h, n, r, x));
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replace_test_12(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() can replace the same sequence of characters";

    pcr_exception_try (x) {
        const pcr_string *test = pcr_string_new("Hello, world!", x);
        const pcr_string *expect = pcr_string_new("Helllo, world!", x);

        pcr_string *repl = pcr_string_replace(test, "l", "ll", x);
        pcr_log_trace("repl = %s", repl);
        return !pcr_string_cmp(repl, expect, x);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replace_test_13(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() throws PCR_EXCEPTION_STRING if passed a NULL"
            " pointer for @needle";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);

        pcr_log_suppress();
        (void) pcr_string_replace(test, NULL, "", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replace_test_14(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @replace";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);

        pcr_log_suppress();
        (void) pcr_string_replace(test, "Hello", NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_replaceall() test cases
 */


static bool
replaceall_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() throws PCR_EXCEPTION_STRING if passed a"
            " null string for @needle";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);

        pcr_log_suppress();
        (void) pcr_string_replaceall(test, "", "Hello, world!", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replaceall_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() replaces an ASCII character with a null"
            " character";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        test = pcr_string_replaceall(test, "!", "", x);

        return !strcmp(test, "Hello, world");
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replaceall_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() replaces a Unicode character with a null"
            " character";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        test = pcr_string_replaceall(test, "т", "", x);

        return !strcmp(test, "Приве, мир!");
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replaceall_test_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() replaces all the instances in an ASCII"
            " string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        test = pcr_string_replaceall(test, "l", "y", x);

        return !strcmp(test, "Heyyo, woryd!");
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replaceall_test_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() replaces all the instances in a Unicode"
            " string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        test = pcr_string_replaceall(test, "р", "r", x);

        return !strcmp(test, "Пrивет, миr!");
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replaceall_test_6(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() replaces an ASCII substring";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        test = pcr_string_replaceall(test, "world", "moon", x);

        return !strcmp(test, "Hello, moon!");
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replaceall_test_7(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() replaces a Unicode substring";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        test = pcr_string_replaceall(test, "Привет", "До свидания", x);

        return !strcmp(test, "До свидания, мир!");
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replaceall_test_8(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() replaces an entire ASCII string";

    pcr_exception_try (x) {
        const pcr_string *expect = "Goodbye, moon?";
        pcr_string *test = pcr_string_new("Hello, world!", x);
        test = pcr_string_replaceall(test, "Hello, world!", expect, x);

        return !strcmp(test, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replaceall_test_9(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() replaces an entire Unicode string";

    pcr_exception_try (x) {
        const pcr_string *expect = "До свидания, луна?";
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        test = pcr_string_replaceall(test, "Привет, мир!", expect, x);

        return !strcmp(test, expect);
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replaceall_test_10(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() throws PCR_EXCEPTION_HANDLE if passed a"
            " NULL pointer for @haystack";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);

        pcr_log_suppress();
        (void) pcr_string_replaceall(NULL, test, "", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replaceall_test_11(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() throws PCR_EXCEPTION_STRING if passed a"
            " NULL pointer for @needle";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);

        pcr_log_suppress();
        (void) pcr_string_replaceall(test, NULL, "", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool
replaceall_test_12(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() throws PCR_EXCEPTION_HANDLE if passed a"
            " NULL pointer for @replace";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);

        pcr_log_suppress();
        (void) pcr_string_replaceall(test, "Hello", NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        pcr_log_allow();
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_int() test cases
 */


static bool
int_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_int() stringifies 0";

    pcr_exception_try (x) {
        return !strcmp(pcr_string_int(0, x), "0");
    }

    pcr_exception_unwind (ex);
    return false;
}


static bool
int_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_int() stringifies a negative integer";

    pcr_exception_try (x) {
        return !strcmp(pcr_string_int(-1024, x), "-1024");
    }

    pcr_exception_unwind (ex);
    return false;
}


static bool
int_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_int() stringifies a positive integer";

    pcr_exception_try (x) {
        return !strcmp(pcr_string_int(1024, x), "1024");
    }

    pcr_exception_unwind (ex);
    return false;
}


/******************************************************************************
 * pcr_string_float() test cases
 */


static bool
float_test_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_float() stringifies 0";

    pcr_exception_try (x) {
        return !strcmp(pcr_string_float(0, x), "0.000000");
    }

    pcr_exception_unwind (ex);
    return false;
}


static bool
float_test_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_float() stringifies a negative floating point number";

    pcr_exception_try (x) {
        return !strcmp(pcr_string_float(-3.141592654, x), "-3.141593");
    }

    pcr_exception_unwind (ex);
    return false;
}


static bool
float_test_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_float() stringifies a positive floating point number";

    pcr_exception_try (x) {
        return !strcmp(pcr_string_float(3.141592654, x), "3.141593");
    }

    pcr_exception_unwind (ex);
    return false;
}


/******************************************************************************
 * pcr_string_testsuite() interface
 */


static pcr_unittest *unit_tests[] = {
    &new_test_1, &new_test_2, &new_test_3, &new_test_4,
    &copy_test_1, &copy_test_2, &copy_test_3, &copy_test_4,
    &len_test_1, &len_test_2, &len_test_3, &len_test_4,
    &sz_test_1, &sz_test_2, &sz_test_3, &sz_test_4,
    &cmp_test_1, &cmp_test_2, &cmp_test_3, &cmp_test_4,
    &cmp_test_5, &cmp_test_6, &cmp_test_7, &cmp_test_8,
    &cmp_test_9, &cmp_test_10, &cmp_test_11, &add_test_1,
    &add_test_2, &add_test_3, &add_test_4, &add_test_5,
    &find_test_1, &find_test_2, &find_test_3, &find_test_4,
    &find_test_5, &find_test_6, &find_test_7, &find_test_8,
    &find_test_9, &find_test_10, &find_test_11, &replace_test_1,
    &replace_test_2, &replace_test_3, &replace_test_4, &replace_test_5,
    &replace_test_6, &replace_test_6, &replace_test_7, &replace_test_7,
    &replace_test_8, &replace_test_9, &replace_test_10, &replace_test_11,
    &replace_test_12, &replace_test_13, &replace_test_14, &replaceall_test_1,
    &replaceall_test_2, &replaceall_test_3, &replaceall_test_4, &replaceall_test_5,
    &replaceall_test_6, &replaceall_test_7, &replaceall_test_8, &replaceall_test_9,
    &replaceall_test_10, &replaceall_test_11, &replaceall_test_12, &int_test_1,
    &int_test_2,         &int_test_3, &float_test_1, &float_test_2,
    &float_test_3
};


extern pcr_testsuite *
pcr_string_testsuite(pcr_exception ex)
{
    pcr_exception_try (x) {
        const pcr_string *name = "PCR String (pcr_string)";
        const size_t len = sizeof unit_tests / sizeof *unit_tests;

        return pcr_testsuite_new_2(name, unit_tests, len, x);
    }

    pcr_exception_unwind(ex);
    return NULL;
}

