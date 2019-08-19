#include "pcr/api.h"

/* This is how you would declare a hot function. */
extern pcr_hint_hot void foo_hot(int x);

/* This is how you would declare a cold function. */
extern pcr_hint_cold bar_cold(int y);

/* The hot function shows how you would indicate a likely predicate. */
void foo_hot(int x)
{
    if (pcr_hint_likely (x != 0))
        printf("x = %d\n", x);
}

/* The cold function shows how you would indicate an unlikely predicate. */
void bar_cold(int y)
{
    if (pcr_hint_unlikely (y > 1))
        printf("y = %d\n", y);
}

