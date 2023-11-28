#ifndef TM_SIM

#define TM_OKAY 0;
#define TM_ACCEPT 2;
#define TM_REJECT 3;

// max size of a single state
// e.g. "q0"
#define STATE_SIZE 8

// number of variables contained in a single step
// e.g. "q0, 0 -> q0, 1, ->"
#define VARS_IN_RULE 5

// max size of single entry in step
// e.g. "->"
#define MAX_RULE_SIZE 16

// max number of steps
#define MAX_RULE_NUM 60


// state of the tm
struct state {
  char  state [STATE_SIZE];
  struct dnode*  band;
  long  band_location;
};

// the turing machine
struct turing {
  short bands;
  char  start  [STATE_SIZE];
  char  accept [STATE_SIZE];
  char  reject [STATE_SIZE];
  char ***steps;
  struct state* state;
};

// creates the turing machine
struct turing* tm_create_machine(char tm[]);

// sets the band for a given tm
int tm_set_band(struct turing* turing, char input_band[]);

// frees the allocated memory
void tm_delete_machine(struct turing* turing);

// runs the tm without stopping
void run_tm(struct turing* turing);

// runs the tm for a single step,
// turing.state.band is the modified cell,
// turing.state.state and itself before running is the used step
// together with band and itself before running
int run_tm_single_step(struct turing* turing);

#endif // !TM_SIM
