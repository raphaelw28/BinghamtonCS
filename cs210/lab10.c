#include <stdio.h>

void mt(int x, int y)
{
  for(int i = 1; i <= x; i = i + 1)
    {
      for(int j = 1; j <= y; j = j + 1)
	{
	  printf("%d", i * j);
	  printf(" ");
	}
      printf("\n");
    }
}

void main()
{
  mt(7, 10);
  printf("\n");
  mt(4,5);
}
