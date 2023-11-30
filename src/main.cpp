#include "tm_sim.h"
#include <cstdio>

int main (int argc, char *argv[]) {
  char def[] = 
    "1;q0;q2;q3;"
    "0,q0->1,q0,->;"
    "_,q0->_,q1,<-;"
    "1,q1->0,q1,<-;"
    "_,q1->_,q2,->"
  ;

  struct turing* tm = tm_create_machine(def);

  char band[] = "0000000";
  tm_set_band(tm, band);

  run_tm_single_step(tm);

  printf("|");

  for (int i = 0; i < MAX_RULE_NUM; ++i) {
    for (int j = 0; j < VARS_IN_RULE; ++j) {
      printf("%s", tm->steps[i][j]);
    }
  }
  
  printf("|\n");
  tm_delete_machine(tm);

  return 0;
}
