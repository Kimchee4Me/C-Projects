/* Implement your code in this file 
   This program prints out a table of squares of a 
   lower limit given to the upper limit */

#include <stdio.h>

int main(void) {
  int lower = 0;
  int upper = 0;
  int i = 0;

  /*ask user for limits*/
  printf("Enter limits: ");
  scanf("%d %d", &lower, &upper);

  /* check if upper>lower */
  if (upper < lower) {
     printf("Invalid values\n");
  }

  else {
     i = lower;

     printf("Squares Table\n");
     
    /* Iterate through and print squares */
     while( i <= upper) {
        printf("| %7d| %7d|\n", i, i*i);
        i++;
     }
  }
  return 0;
}
