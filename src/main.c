#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// here are the defining features of the tm 
struct tm {
  short bands     ;
  char  start  [3];
  char  accept [3];
  char  reject [3];
};


// this function assumes arr contains only numbers
int buf_sum(char arr[]) {
  int sum = 0;
  int pot = 0;

  for (int i = strlen(arr)-1; i > -1; --i) {
    sum += (arr[i] - 48) * (int) pow(10, pot++);
  }

  return sum;
}


// there is no reason not to reuse the same buffer
// (not that i can think of one at least)
void buf_reset(char arr[]) {
  for (int i = 0; i < strlen(arr); ++i) {
    if (arr[i] == '\0') {
      return;
    }
    arr[i] = '\0';
  }
}


int main(int argc, char *argv[]) {
  char input[] = "11235; q0; q3; q4;"
    "q0, 0 -> q0, 0, > ;"
    "q0, 1 -> q0, 1, > ;"
    "q0, _ -> q1, _, < ;"
    "q1, 0 -> q2, 1, < ;"
    "q1, 1 -> q1, 0, < ;"
    "q1, _ -> q3, 1, - ;"
    "q2, 1 -> q2, 1, < ;"
    "q2, 0 -> q2, 0, < ;"
    "q2, _ -> q3, _, >";

  struct tm tim = {0, {'\0'}, {'\0'}, {'\0'}};
  
  char buf[10] = {'\0'};

  for (int i = 0; i < INT_MAX; ++i) {
    if (input[i] == ';') {
      tim.bands = buf_sum(buf);
      buf_reset(buf);
      break;    
    }
    
    buf[i] = input[i];
  }
  
  printf("%i\n", tim.bands);
  
}
