#include <string.h>
#include "./suites.h"


/******************************************************************************
 * pcr_string_new() test cases
 */


static bool test_new_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_new() can create an empty string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        return test && !*test;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_new_2(pcr_string **desc, pcr_exception ex)
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


static bool test_new_3(pcr_string **desc, pcr_exception ex)
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


static bool test_new_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_new() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @cstr";

    pcr_exception_try (x) {
        (void) pcr_string_new(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_copy() test cases
 */


static bool test_copy_1(pcr_string **desc, pcr_exception ex)
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


static bool test_copy_2(pcr_string **desc, pcr_exception ex)
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


static bool test_copy_3(pcr_string **desc, pcr_exception ex)
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


static bool test_copy_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_copy() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_string_copy(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_len() test cases
 */


static bool test_len_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_len() reports 0 for a null string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        return pcr_string_len(test, x) == 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_len_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_len() reports the length of an ASCII string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        return pcr_string_len(test, x) == 13;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_len_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_len() reports the length of a Unicode string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        return pcr_string_len(test, x) == 12;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_len_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_len() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_string_len(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_sz() test cases
 */


static bool test_sz_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_sz() reports 1 for a null string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        return pcr_string_sz(test, x) == 1;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_sz_2(pcr_string **desc, pcr_exception ex)
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


static bool test_sz_3(pcr_string **desc, pcr_exception ex)
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


static bool test_sz_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_sz() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @ctx";

    pcr_exception_try (x) {
        (void) pcr_string_sz(NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_cmp() test cases
 */


static bool test_cmp_1(pcr_string **desc, pcr_exception ex)
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


static bool test_cmp_2(pcr_string **desc, pcr_exception ex)
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


static bool test_cmp_3(pcr_string **desc, pcr_exception ex)
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


static bool test_cmp_4(pcr_string **desc, pcr_exception ex)
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


static bool test_cmp_5(pcr_string **desc, pcr_exception ex)
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


static bool test_cmp_6(pcr_string **desc, pcr_exception ex)
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


static bool test_cmp_7(pcr_string **desc, pcr_exception ex)
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


static bool test_cmp_8(pcr_string **desc, pcr_exception ex)
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


static bool test_cmp_9(pcr_string **desc, pcr_exception ex)
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


static bool test_cmp_10(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_cmp() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @lhs";

    pcr_exception_try (x) {
        (void) pcr_string_cmp(NULL, "Hello, world!", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_cmp_11(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_cmp() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @rhs";

    pcr_exception_try (x) {
        (void) pcr_string_cmp("Hello, world!", NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_add() test cases
 */


static bool test_add_1(pcr_string **desc, pcr_exception ex)
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


static bool test_add_2(pcr_string **desc, pcr_exception ex)
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


static bool test_add_3(pcr_string **desc, pcr_exception ex)
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


static bool test_add_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_add() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @ctx";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello", x);
        (void) test;

        test = pcr_string_add(NULL, ", ", x);
        (void) test;
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_add_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_add() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @add";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello", x);
        test = pcr_string_add(test, NULL, x);
        (void) test;
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_find() test cases
 */


static bool test_find_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can find a null string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        return pcr_string_find(test, "", x) == 1;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_find_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can find an ASCII string with 1 character";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        return pcr_string_find(test, "w", x) == 8;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_find_3(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can find a Unicode string with 1 character";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        return pcr_string_find(test, "м", x) == 9;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_find_4(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can find an ASCII string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        return pcr_string_find(test, "world", x) == 8;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_find_5(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can find a Unicode string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        return pcr_string_find(test, "мир", x) == 9;
    }

    pcr_exception_unwind(ex);
    return false;
}



static bool test_find_6(pcr_string **desc, pcr_exception ex)
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


static bool test_find_7(pcr_string **desc, pcr_exception ex)
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


static bool test_find_8(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can detect the absence of an ASCII string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        return pcr_string_find(test, "moon", x) == 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_find_9(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() can detect the absence of a Unicode string";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Привет, мир!", x);
        return pcr_string_find(test, "луна", x) == 0;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_find_10(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @haystack";

    pcr_exception_try (x) {
        (void) pcr_string_find(NULL, "Hello", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_find_11(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_find() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @needle";

    pcr_exception_try (x) {
        (void) pcr_string_find("Hello", NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_replace() test cases
 */


static bool test_replace_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() throws PCR_EXCEPTION_STRING if passed a null"
            " string for @needle";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        (void) pcr_string_replace(test, "", "Hello, world!", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_replace_2(pcr_string **desc, pcr_exception ex)
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


static bool test_replace_3(pcr_string **desc, pcr_exception ex)
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


static bool test_replace_4(pcr_string **desc, pcr_exception ex)
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


static bool test_replace_5(pcr_string **desc, pcr_exception ex)
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


static bool test_replace_6(pcr_string **desc, pcr_exception ex)
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


static bool test_replace_7(pcr_string **desc, pcr_exception ex)
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


static bool test_replace_8(pcr_string **desc, pcr_exception ex)
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


static bool test_replace_9(pcr_string **desc, pcr_exception ex)
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


static bool test_replace_10(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @haystack";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        (void) pcr_string_replace(NULL, test, "", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_replace_11(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() throws PCR_EXCEPTION_STRING if passed a NULL"
            " pointer for @needle";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        (void) pcr_string_replace(test, NULL, "", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_replace_12(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replace() throws PCR_EXCEPTION_HANDLE if passed a NULL"
            " pointer for @replace";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        (void) pcr_string_replace(test, "Hello", NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_replaceall() test cases
 */


static bool test_replaceall_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() throws PCR_EXCEPTION_STRING if passed a"
            " null string for @needle";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("", x);
        (void) pcr_string_replaceall(test, "", "Hello, world!", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_replaceall_2(pcr_string **desc, pcr_exception ex)
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


static bool test_replaceall_3(pcr_string **desc, pcr_exception ex)
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


static bool test_replaceall_4(pcr_string **desc, pcr_exception ex)
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


static bool test_replaceall_5(pcr_string **desc, pcr_exception ex)
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


static bool test_replaceall_6(pcr_string **desc, pcr_exception ex)
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


static bool test_replaceall_7(pcr_string **desc, pcr_exception ex)
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


static bool test_replaceall_8(pcr_string **desc, pcr_exception ex)
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


static bool test_replaceall_9(pcr_string **desc, pcr_exception ex)
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


static bool test_replaceall_10(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() throws PCR_EXCEPTION_HANDLE if passed a"
            " NULL pointer for @haystack";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        (void) pcr_string_replaceall(NULL, test, "", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_replaceall_11(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() throws PCR_EXCEPTION_STRING if passed a"
            " NULL pointer for @needle";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        (void) pcr_string_replaceall(test, NULL, "", x);
    }

    pcr_exception_catch (PCR_EXCEPTION_STRING) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


static bool test_replaceall_12(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_replaceall() throws PCR_EXCEPTION_HANDLE if passed a"
            " NULL pointer for @replace";

    pcr_exception_try (x) {
        pcr_string *test = pcr_string_new("Hello, world!", x);
        (void) pcr_string_replaceall(test, "Hello", NULL, x);
    }

    pcr_exception_catch (PCR_EXCEPTION_HANDLE) {
        return true;
    }

    pcr_exception_unwind(ex);
    return false;
}


/******************************************************************************
 * pcr_string_int() test cases
 */


static bool
test_int_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_int() stringifies 0";

    pcr_exception_try (x) {
        return !strcmp(pcr_string_int(0, x), "0");
    }

    pcr_exception_unwind (ex);
    return false;
}


static bool
test_int_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_int() stringifies a negative integer";

    pcr_exception_try (x) {
        return !strcmp(pcr_string_int(-1024, x), "-1024");
    }

    pcr_exception_unwind (ex);
    return false;
}


static bool
test_int_3(pcr_string **desc, pcr_exception ex)
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
test_float_1(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_float() stringifies 0";

    pcr_exception_try (x) {
        return !strcmp(pcr_string_float(0, x), "0.000000");
    }

    pcr_exception_unwind (ex);
    return false;
}


static bool
test_float_2(pcr_string **desc, pcr_exception ex)
{
    *desc = "pcr_string_float() stringifies a negative floating point number";

    pcr_exception_try (x) {
        return !strcmp(pcr_string_float(-3.141592654, x), "-3.141593");
    }

    pcr_exception_unwind (ex);
    return false;
}


static bool
test_float_3(pcr_string **desc, pcr_exception ex)
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
    test_new_1, test_new_2, test_new_3, test_new_4, test_copy_1, test_copy_2,
    test_copy_3, test_copy_4, test_len_1, test_len_2, test_len_3, test_len_4,
    test_sz_1, test_sz_2, test_sz_3, test_sz_4, test_cmp_1, test_cmp_2,
    test_cmp_3, test_cmp_4, test_cmp_5, test_cmp_6, test_cmp_7, test_cmp_8,
    test_cmp_9, test_cmp_10, test_cmp_11, test_add_1, test_add_2, test_add_3,
    test_add_4, test_add_5, test_find_1, test_find_2, test_find_3, test_find_4,
    test_find_5, test_find_6, test_find_7, test_find_8, test_find_9,
    test_find_10, test_find_11, test_replace_1, test_replace_2, test_replace_3,
    test_replace_4, test_replace_5, test_replace_6, test_replace_6,
    test_replace_7, test_replace_7, test_replace_8, test_replace_9,
    test_replace_10, test_replace_11, test_replace_12, test_replaceall_1,
    test_replaceall_2, test_replaceall_3, test_replaceall_4, test_replaceall_5,
    test_replaceall_6, test_replaceall_7, test_replaceall_8, test_replaceall_9,
    test_replaceall_10, test_replaceall_11, test_replaceall_12, test_int_1,
    test_int_2, test_int_3, test_float_1, test_float_2, test_float_3
};


extern pcr_testsuite *
pcr_string_testsuite(pcr_exception ex)
{
    pcr_exception_try (x) {
        pcr_testsuite *ts = pcr_testsuite_new("PCR String (pcr_string)", x);

        register size_t len = sizeof unit_tests / sizeof *unit_tests;
        for (register size_t i = 0; i < len; i++)
            pcr_testsuite_push(ts, pcr_testcase_new(unit_tests[i], x), x);

        return ts;
    }

    pcr_exception_unwind(ex);
    return NULL;
}

