#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


// BUFSIZE has to be larger than STATE_SIZE
// size of parsing buffer
#define BUFSIZE 16

// max size of a single state
#define STATE_SIZE 8

// maximum input string (tm definition) length
#define MAX_INPUT_SIZE 1024

// max number of steps
#define MAX_STEPS_NUM 60

// max size of single entry in step
#define MAX_STEPS_SIZE 16

//
#define VARS_IN_STEP 5


// here are the defining features of the tm 
struct tm {
  short bands               ;
  char  start  [STATE_SIZE] ;
  char  accept [STATE_SIZE] ;
  char  reject [STATE_SIZE] ;
};


void clean_input(char input[]) {
  // create new string where input gets parsed into
  char rem[MAX_INPUT_SIZE] = {'\0'};

  int rem_index = 0; // tracks index of next char
  int input_length = strlen(input); // just read the code

  for (int i = 0; i < input_length; ++i) {
    if (input[i] == ' ' || input[i] == '\n') {
      continue;
    }
    
    // copy the char if it isnt one of the above
    rem[rem_index++] = input[i];
  }

  memset(input, '\0', input_length); // reset input
  memcpy(input, rem, strlen(rem)); // save cleaned input
}


// this function assumes arr contains only numbers
// converts string representation of integer to int
// i.e. "132" to 132
int buf_sum(char arr[]) {
  int sum = 0;
  int pot = 0;

  for (int i = strlen(arr)-1; i > -1; --i) {
    sum += (arr[i] - 48) * (int) pow(10, pot++);
  }

  return sum;
}


void buf_reset(char arr[]) {
  memset(arr, '\0', strlen(arr));
}


void parse_definition(char arr[], struct tm* tim) {
  char buf[BUFSIZE] = {'\0'}; // buffer
  short buf_index = 0; // tracks next char in buffer
  short sem_count = 0; // tracks count of semicolons

  for (int i = 0; i < strlen(arr); ++i) {
    if (arr[i] == ';') {
      switch (sem_count) {
        case 0: // copy over amount of bands
          (*tim).bands = buf_sum(buf);
          break;

        case 1: // copy over starting state
          memcpy((*tim).start, buf, STATE_SIZE-1);
          break;

        case 2: // copy over accepting state
          memcpy((*tim).accept, buf, STATE_SIZE-1);
          break;

        case 3: // copy over rejecting state
          memcpy((*tim).reject, buf, STATE_SIZE-1);
          return;
      }
      
      buf_reset(buf); // reset buffer
      buf_index = 0; // reset buffer index

      sem_count++; // increase semicolon count
      i++; // skip the semicolon
    }    

    buf[buf_index++] = arr[i]; // copy next char to buffer
  }
}


void parse_steps(char steps[MAX_STEPS_NUM][VARS_IN_STEP][MAX_STEPS_SIZE], char input[]) {
  char buf[BUFSIZE] = {'\0'}; // buffer
  short buf_index = 0; // tracks index of next char in buffer 
  short sem_count = 0; // tracks the number of semicolons counted
  short input_start_index = 0; // index of first char after definition
  short sym_count = 0; // counts ',' and '-' symbols

  // get index where tm steps start
  do {
    if (input[input_start_index++] == ';') { sem_count++; }
  } while (sem_count < 4);

  sem_count = 0; // reset count for context switch

  for (int i = input_start_index; i < strlen(input); ++i) {
    // buf_reset cant be default
    switch (input[i]) {
      case '-':
        i++; // same as with ',' but need to skip the two symbols "->"

      case ',':
        i++; // skip the symbol for next read to buffer
        
        // copy buffer to current step at semicolon_count, symbol_count
        memcpy(steps[sem_count][sym_count++], buf, strlen(buf));
        
        buf_reset(buf); // reset buffer
        buf_index = 0; // reset buffer index
        break;

      case ';':
        i++; // skip the symbol
        
        // copy buffer to current step at semicolon_count, symbol_count
        memcpy(steps[sem_count][sym_count++], buf, strlen(buf));
        
        sem_count++; // end of step so goto next
        sym_count = 0; // reset symbol count for next step

        buf_reset(buf); // reset buffer
        buf_index = 0; // reset buffer index
        
        break;
    }

    buf[buf_index++] = input[i]; // copy next char to buffer
  }

  // copy remaining step member over
  memcpy(steps[sem_count][sym_count++], buf, strlen(buf));
}


int main(int argc, char *argv[]) {
  // for testing purposes
  // tm that increments the input
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
  
  // clean up input
  clean_input(input);

  // input needs to be smaller than that arbitrary input max
  if (strlen(input) > MAX_INPUT_SIZE) {
    printf("Input Size too large!\nCurrent max: %i\n", MAX_INPUT_SIZE);
    return 1;
  }

  // parse defining features of tm
  struct tm tim = {0, {'\0'}, {'\0'}, {'\0'}};
  parse_definition(input, &tim);
  
  // parse conversion steps of tm
  char steps[MAX_STEPS_NUM][5][MAX_STEPS_SIZE] = {{{'\0'}}};
  parse_steps(steps, input);

  /*
   * DEBUG PRINTING TO STDOUT
   */
  printf("%i %s %s %s\n", tim.bands, tim.start, tim.accept, tim.reject);
  
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < VARS_IN_STEP; ++j) {
    printf("%s ", steps[i][j]);
    }
    printf("\n");
  }
}

