//Raphael Wong
#include <stdio.h>

int main()
{
  int row, column;

  for (row = 1; row <= 5; row = row + 1)
    {
      for (column = 1; column <= 5; column = column + 1)
      {
      printf(" %4d", row * column);

    }
  printf("\n");
}

}
