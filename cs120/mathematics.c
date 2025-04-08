#include <stdio.h>

typedef struct
{
    int a;
    int b;
} int_struct;

typedef struct
{
    int a;
    char b;
} mixed_struct;

int main()
{
    int_struct a[10];
    mixed_struct b[10];

    printf("Addresses A %p %p\n", &a[1], &a[0]);
    printf("Addresses B %p %p\n", &b[1], &b[0]);
}
