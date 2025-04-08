// Name: Raphael
#include <stdio.h>

int main()
{
  int x;
  printf("Enter a number: ");
  scanf("%i", &x);

  for(int i = 7; i >= 0; i--)
    {
      int bit = x & (1 << i);
      if (bit != 0)
	printf("1");
      else
	printf("0");

    }
  printf("\n");
}

      
