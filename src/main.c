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

// number of variables contained in a single step
#define VARS_IN_STEP 5


// here are the defining features of the tm 
struct tm {
  short bands;
  char  start  [STATE_SIZE];
  char  accept [STATE_SIZE];
  char  reject [STATE_SIZE];
};


struct state {
  char  state [STATE_SIZE]    ;
  char  band  [MAX_INPUT_SIZE];
  long  band_location;
};


struct tm tim = {0, {'\0'}, {'\0'}, {'\0'}};
char steps[MAX_STEPS_NUM][VARS_IN_STEP][MAX_STEPS_SIZE] = {{{'\0'}}};


void clean_string(char input[], char io) {
  char c1 = ' ';
  char c2 = '\n';

  if (io == 'o') {
    c1 = '_';
    c2 = '_';
  }

  // create new string where input gets parsed into
  char rem[MAX_INPUT_SIZE] = {'\0'};

  int rem_index = 0; // tracks index of next char
  int input_length = strlen(input); // just read the code

  for (int i = 0; i < input_length; ++i) {
    if (input[i] == c1 || input[i] == c2) {
      continue;
    }
    
    // copy the char if it isnt one of the above
    rem[rem_index++] = input[i];
  }

  memset(input, '\0', input_length); // reset input
  memcpy(input, rem, strlen(rem)); // save cleaned input
}


void clean_input(char input[]) {
  clean_string(input, 'i');
}


void clean_output(char output[]) {
  clean_string(output, 'o');
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


void parse_definition(char input[]) {
  char buf[BUFSIZE] = {'\0'}; // buffer
  short buf_index = 0; // tracks next char in buffer
  short sem_count = 0; // tracks count of semicolons

  for (int i = 0; i < strlen(input); ++i) {
    if (input[i] == ';') {
      switch (sem_count) {
        case 0: // copy over amount of bands
          tim.bands = buf_sum(buf);
          break;

        case 1: // copy over starting state
          memcpy(tim.start, buf, STATE_SIZE-1);
          break;

        case 2: // copy over accepting state
          memcpy(tim.accept, buf, STATE_SIZE-1);
          break;

        case 3: // copy over rejecting state
          memcpy(tim.reject, buf, STATE_SIZE-1);
          return;
      }
      
      buf_reset(buf); // reset buffer
      buf_index = 0; // reset buffer index

      sem_count++; // increase semicolon count
      i++; // skip the semicolon
    }    

    buf[buf_index++] = input[i]; // copy next char to buffer
  }
}


void parse_steps(char input[]) {
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


// returns the index in steps array of the applicable step
void run_tm(struct state* state) {
  // while neither accepting or rejecting state are reached
  while (strcmp(tim.accept, (*state).state) && strcmp(tim.reject, (*state).state)) {
    // go over every possible step and
    for (int i = 0; i < MAX_STEPS_NUM; ++i) {
      char band = 
        (*state).band_location > strlen((*state).band) -1 ||
        (*state).band_location < 0 
        ? '_' : (*state).band[(*state).band_location];
      
      // if satisfying below conditions, execute step
      if (!strcmp(steps[i][0], (*state).state) && band == steps[i][1][0]) {
        // print the used step
        // printf("%s %s -> %s %s %s\n", steps[i][0], steps[i][1], steps[i][2], steps[i][3], steps[i][4]);

        // shift entire band right if modifying left of band
        if ((*state).band_location < 0) {
          long band_size = strlen((*state).band);

          for (int i = band_size -1; i > -1; --i) {
            (*state).band[i+1] = (*state).band[i];
          }

          (*state).band_location++;
        }

        (*state).band[(*state).band_location] = steps[i][3][0];
        memset((*state).state, '\0', STATE_SIZE);
        strcpy((*state).state, steps[i][2]);

        switch (steps[i][4][0]) {
          case '>':
            (*state).band_location++;
            break;
          
          case '<':
            (*state).band_location--;
            break;
          
          case '-':
            // no change
            break;
        }

        break;
      }
    }
  }
}


// runs the simulator
void start_tm(char tm[], char input_band[], char output_band[]) {
  memset(output_band, '\0', strlen(output_band));

  if (tim.start == tim.accept) {
    printf("do you actually want me to do something?\n");
    return;

  } else if (strlen(input_band) > MAX_INPUT_SIZE) {
    printf("input too big, aborting\n");
    return;
  }

  // clean up input
  clean_input(tm);
  clean_input(input_band);

  // parse defining features of tm
  parse_definition(tm);
  
  // parse conversion steps of tm
  parse_steps(tm);

  char output[MAX_INPUT_SIZE] = {'\0'};
  struct state state = {{'\0'}, {'\0'}, 0};

  strcpy(state.state, tim.start);
  strcpy(state.band, input_band);
  
  run_tm(&state);
  
  clean_output(state.band);
  strcpy(output_band, state.band);
}


int main(int argc, char *argv[]) {
  // tm that increments the input
  char tm1[] =
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
  
  // tm that decrements the input
  char tm2[] =
    "1; q0; q3; q4;"
    "q0, 0 -> q0, 0, >;"
    "q0, 1 -> q6, 1, >;"
    "q0, _ -> q4, _, -;"
    "q6, 0 -> q6, 0, >;"
    "q6, 1 -> q6, 1, >;"
    "q6, _ -> q1, _, <;"
    "q1, 0 -> q2, 1, <;"
    "q1, 1 -> q5, 0, <;"
    "q1, _ -> q4, _, -;"
    "q2, 0 -> q2, 1, <;"
    "q2, 1 -> q5, 0, <;"
    "q5, 0 -> q5, 0, <;"
    "q5, 1 -> q5, 1, <;"
    "q5, _ -> q3, _, >";

  // tm that doubles the input
  char tm3[] =
    "1; q0; q2; q3;"
    "q0, 0 -> q0, 0, >;"
    "q0, 1 -> q0, 1, >;"
    "q0, _ -> q1, 0, <;"
    "q1, 0 -> q1, 0, <;"
    "q1, 1 -> q1, 1, <;"
    "q1, _ -> q2, _, >";
  
  char output[MAX_INPUT_SIZE] = {'\0'};

  char band1[] = "0110 1001 0101 1010 1111 0000 1111 0000";
  char band2[] = "1001 0110 1010 0101 0110 1001 0101 1010";
  char band3[] = "1111 0000 1111 0000 1001 0110 1010 0101";

  //-----------------------------------------
  printf("INCREMENT\n");

  start_tm(tm1, band1, output);
  printf("INP: %s\nOUT: %s\n\n", band1, output);
  memset(output, '\0', strlen(output));

  start_tm(tm1, band2, output);
  printf("INP: %s\nOUT: %s\n\n", band2, output);
  memset(output, '\0', strlen(output));
  
  start_tm(tm1, band3, output);
  printf("INP: %s\nOUT: %s\n\n", band3, output);
  memset(output, '\0', strlen(output));
  
  //-----------------------------------------
  printf("\n\nDECREMENT\n");

  start_tm(tm2, band1, output);
  printf("INP: %s\nOUT: %s\n\n", band1, output);
  memset(output, '\0', strlen(output));
  
  start_tm(tm2, band2, output);
  printf("INP: %s\nOUT: %s\n\n", band2, output);
  memset(output, '\0', strlen(output));
  
  start_tm(tm2, band3, output);
  printf("INP: %s\nOUT: %s\n\n", band3, output);
  memset(output, '\0', strlen(output));

  //-----------------------------------------
  printf("\n\nFUNCTION 2*X\n");
  
  start_tm(tm3, band1, output);
  printf("INP: %s\nOUT: %s\n\n", band1, output);
  memset(output, '\0', strlen(output));
  
  start_tm(tm3, band2, output);
  printf("INP: %s\nOUT: %s\n\n", band2, output);
  memset(output, '\0', strlen(output));
  
  start_tm(tm3, band3, output);
  printf("INP: %s\nOUT: %s\n\n", band3, output);
  memset(output, '\0', strlen(output));
  //-----------------------------------------
}

