#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "util/dllist.h"
#include "tm_sim.h"


void clean_string(char *input, const char io) {
  char c1 = ' ';
  char c2 = '\n';

  if (io == 'o') {
    c1 = '_';
    c2 = '_';
  }

  // create new string where input gets parsed into
  char* rem = (char*) malloc(strlen(input) * sizeof(char));

  int rem_index = 0; // tracks index of next char
  int input_length = strlen(input); // just read the code

  for (int i = 0; i < input_length; ++i) {
    if (input[i] == c1 || input[i] == c2) {
      continue;
    }
    
    // copy the char if it isnt one of the above
    rem[rem_index++] = input[i];
    input[i] = '\0';
  }

  memcpy(input, rem, strlen(rem)); // save cleaned input
  free(rem);
}


void clean_input(char *input) {
  clean_string(input, 'i');
}


void clean_output(char *output) {
  clean_string(output, 'o');
}


// this function assumes arr contains only numbers
// converts string representation of integer to int
// i.e. "132" to 132
int buf_sum(char* arr) {
  int sum = 0;
  int pot = 0;

  for (int i = strlen(arr)-1; i > -1; --i) {
    sum += (arr[i] - 48) * (int) pow(10, pot++);
  }

  return sum;
}


void buf_reset(char* arr) {
  memset(arr, '\0', strlen(arr));
}


void parse_definition(struct turing* turing, char input[]) {
  char buf[STATE_SIZE] = {'\0'}; // buffer
  short buf_index = 0; // tracks next char in buffer
  short sem_count = 0; // tracks count of semicolons

  for (int i = 0; i < strlen(input); ++i) {
    if (input[i] == ';') {
      switch (sem_count) {
        case 0: // copy over amount of bands
          turing -> bands = buf_sum(buf);
          break;

        case 1: // copy over starting state
          memcpy(turing -> start, buf, STATE_SIZE);
          break;

        case 2: // copy over accepting state
          memcpy(turing -> accept, buf, STATE_SIZE);
          break;

        case 3: // copy over rejecting state
          memcpy(turing -> reject, buf, STATE_SIZE);
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


void parse_steps(struct turing* turing, char input[]) {
  char buf[STATE_SIZE] = {'\0'}; // buffer
  memset(buf, '\0', STATE_SIZE);

  short buf_index = 0; // tracks index of next char in buffer 
  short sem_count = 0; // tracks the number of semicolons counted
  short input_start_index = 0; // index of first char after definition
  short sym_count = 0; // counts ',' and '-' symbols

  char ***steps = (char***) malloc(MAX_RULE_NUM * sizeof(char**));

  for (int i = 0; i < MAX_RULE_NUM; ++i) {
    steps[i] = (char**) malloc(VARS_IN_RULE * sizeof(char*));
    for (int j = 0; j < VARS_IN_RULE; ++j) {
      steps[i][j] = (char*) malloc(STATE_SIZE * sizeof(char));
    }
  }

  // get index where tm steps start
  do {
    if (input[input_start_index++] == ';') { sem_count++; }
  } while (sem_count < 4);

  sem_count = 0; // reset count for context switch

  for (int i = input_start_index; i < strlen(input); ++i) {
    // buf_reset cant be defaultX_RULE_SIZE
    switch (input[i]) {
      case '-':
        // next symbol must be ';'
        // do everything in ';' case and break
        if (input[i-1] == '<') {
          // copy buffer to current step at semicolon_count, symbol_count
          memcpy(steps[sem_count][sym_count++], buf, strlen(buf));

          sem_count++; // end of step so goto next
          sym_count = 0; // reset symbol count for next step

          buf_reset(buf); // reset buffer
          buf_index = 0; // reset buffer index
          
          // skip semikolon
          i += 2;
          break;
        }

        // next symbol must be '>'
        if (input[i-1] == ',')
          buf[buf_index] = '>';
        
        //skip next symbol
        i++;

        // do all in next step below

      case ',':
        i++; // skip the symbol for next read to buffer if for

        // copy buffer to current step at semicolon_count, symbol_count
        memcpy(steps[sem_count][sym_count++], buf, STATE_SIZE);
        
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

  turing -> steps = steps;
}


void run_tm(struct turing* turing) {
  struct dnode* band = turing->state->band;
  char *accept = turing->accept;
  char *reject = turing->reject;
  struct state* state = turing->state;
  char ***steps = turing->steps;

  // while neither accepting or rejecting state are reached
  while (strcmp(accept, state->state) && strcmp(reject, state->state)) {
    // go over every possible step and
    for (int i = 0; i < MAX_RULE_NUM; ++i) {
      // if satisfying below conditions, execute step
      if (!strcmp(steps[i][0], state->state) && band->value == steps[i][1][0]) {
        // print the used step
        // printf("%s %s -> %s %s %s\n", steps[i][0], steps[i][1], steps[i][2], steps[i][3], steps[i][4]);
        band->value = steps[i][3][0];

        memset(state->state, '\0', STATE_SIZE);
        strcpy(state->state, steps[i][2]);

        switch (steps[i][4][0]) {
          case '>':
            if (band->next == NULL) {
              dlist_append(band, '_');
            }
            band = band->next;
            break;
          
          case '<':
            if (band->prev == NULL) {
              dlist_prepend(band, '_');
            }
            band = band->prev;
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


int run_tm_single_step(struct turing* turing) {
  struct dnode* band = turing->state->band;
  char *accept = turing->accept;
  char *reject = turing->reject;
  struct state* state = turing->state;
  char ***steps = turing->steps;

  if (strcmp(accept, state->state))
    return TM_ACCEPT;

  if (strcmp(reject, state->state))
    return TM_REJECT;

  // go over every possible step and
  for (int i = 0; i < MAX_RULE_NUM; ++i) {
    // if satisfying below conditions, execute step
    if (!strcmp(steps[i][0], state->state) && band->value == steps[i][1][0]) {
      // print the used step
      band->value = steps[i][3][0];

      memset(state->state, '\0', STATE_SIZE);
      strcpy(state->state, steps[i][2]);

      switch (steps[i][4][0]) {
        case '>':
          if (band->next == NULL) {
            dlist_append(band, '_');
          }
          band = band->next;
          break;
        
        case '<':
          if (band->prev == NULL) {
            dlist_prepend(band, '_');
          }
          band = band->prev;
          break;
        
        case '-':
          // no change
          break;
      }
    }

    break;
  }

  return TM_OKAY;
}


struct turing* tm_create_machine(char *tm) {
  struct turing* turing = (struct turing*) malloc(sizeof(struct turing));

  // clean input and convert input to dlist
  clean_input(tm);
  
  // parse input into tm
  parse_definition(turing, tm);
  parse_steps(turing, tm);
  
  // create starting state
  struct state* state = (struct state*) malloc(sizeof(struct state));
  state -> band_location = 0;
  state -> band = NULL;
  strcpy(state -> state, turing -> start);

  turing -> state = state;
  
  return turing;
}


int tm_set_band(struct turing* turing, char *input_band) {
  clean_input(input_band);

  if (turing -> state -> band != NULL) {
    dlist_free(turing -> state -> band);
    free(turing -> state);
    turing -> state = NULL;
  }

  struct dnode* band_list = dlist_from_string(input_band);
  turing -> state -> band = band_list;

  return TM_OKAY;
}


void tm_delete_machine(struct turing* turing) {
  free(turing -> steps);
  free(turing -> state);
  free(turing);
}

