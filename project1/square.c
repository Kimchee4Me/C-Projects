/* This program prints out a table of squares, given a lower and upper limit
   from the user; */
#include stdio.h
int main(void) {
  int lower = 0;
  int upper = 0;
  int i = 0;

  /*ask user for limits*/
  printf("Enter limits: ");
  scanf("%d %d" , &lower, &upper);

  /*Check if upper is less than lower limit */
  if (upper < lower) {
    printf("Invalid values\n");
  }

  /*Iterate through and print squares*/
  else {
    i = lower;
    
    while (i <= upper) {
      printf("| %7d| %7d|\n", i, i*i);
      i++;
    }
  }

  return 0;
}
