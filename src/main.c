#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// BUFSIZE has to be larger than STATE_SIZE
#define BUFSIZE 32
#define STATE_SIZE 8

// maximum input string (tm definition) length
#define MAX_INPUT_SIZE 1024


// here are the defining features of the tm 
struct tm {
  short bands               ;
  char  start  [STATE_SIZE] ;
  char  accept [STATE_SIZE] ;
  char  reject [STATE_SIZE] ;
};


void remove_whitespaces(char input[]) {
  char rem[MAX_INPUT_SIZE] = {'\0'};
  int rem_index = 0;
  int input_length = strlen(input);

  for (int i = 0; i < input_length; ++i) {
    if (input[i] != ' ') {
      rem[rem_index++] = input[i];
    }
  }

  memset(input, '\0', input_length);
  
  memcpy(input, rem, strlen(rem));

}


// this function assumes arr contains only numbers
int buf_sum(char arr[]) {
  int sum = 0;
  int pot = 0;

  for (int i = strlen(arr)-1; i > -1; --i) {
    sum += (arr[i] - 48) * (int) pow(10, pot++);
  }

  return sum;
}


void buf_reset(char arr[]) {
  for (int i = 0; i < BUFSIZE; ++i) {
    if (arr[i] == '\0') { return; }
    arr[i] = '\0';
  }
}


void parse_definition(char arr[], struct tm* tim) {
  char buf[BUFSIZE] = {'\0'};
  short buf_index = 0;
  short sem_count = 0;

  for (int i = 0; i < INT_MAX; ++i) {
    if (arr[i] == ';') {
      switch (sem_count) {
        case 0:
          (*tim).bands = buf_sum(buf);
          buf_reset(buf);
          sem_count++;
          buf_index = 0;
          break;

        case 1:
          memcpy((*tim).start, buf, STATE_SIZE-1);
          buf_reset(buf);
          sem_count++;
          buf_index = 0;
          break;

        case 2:
          memcpy((*tim).accept, buf, STATE_SIZE-1);
          buf_reset(buf);
          sem_count++;
          buf_index = 0;
          break;

        case 3:
          memcpy((*tim).reject, buf, STATE_SIZE-1);
          return;
      }
      i++; // skip the semicolon
    }    

    buf[buf_index++] = arr[i];
  }
}


int main(int argc, char *argv[]) {
  char input[] = 
    "1; q0; q3; q4;"
    "q0, 0 -> q0, 0, > ;"
    "q0, 1 -> q0, 1, > ;"
    "q0, _ -> q1, _, < ;"
    "q1, 0 -> q2, 1, < ;"
    "q1, 1 -> q1, 0, < ;"
    "q1, _ -> q3, 1, - ;"
    "q2, 1 -> q2, 1, < ;"
    "q2, 0 -> q2, 0, < ;"
    "q2, _ -> q3, _, >";

  remove_whitespaces(input);

  struct tm tim = {0, {'\0'}, {'\0'}, {'\0'}};
  
  parse_definition(input, &tim);
  printf("%i : %s : %s : %s\n", tim.bands, tim.start, tim.accept, tim.reject);
  
}

