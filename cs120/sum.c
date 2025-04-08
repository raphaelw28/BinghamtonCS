// Calculate the sum of the numbers 1 to MAX
// Raphael Wong
#include <stdio.h>

#define MAX 10

int main()
{
  int sum, i;

  sum = 0;
  for (i = 1; i <= MAX; i = i + 1)
    sum = sum + i;

    printf("The sum from 1 is %d is %d\n",
	   MAX, sum);
}
