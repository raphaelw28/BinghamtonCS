#include <stdio.h>
int main(int argc, char *argv[])
{
    int total = 0;

    for(int i = 1; i < argc; i++)
    {
        int v;

        printf("Argument %d is >%s<\n", i, argv[i]);
        sscanf(argv[i], "%i", &v);
        printf("The integer value for argument %d is %d (0x%x in hex)\n", i, v, v);
        total = total + v;
    }

    printf("The total is %d, or 0x%x in hex.\n", total, total);
}
