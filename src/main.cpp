#include "tm_sim.h"

int main (int argc, char *argv[]) {
  char def[] = 
    "1;q0;q2;q3;"
    "0,q0->1,q0,->;"
    "_,q0->_,q1,<-;"
    "1,q1->0,q1,<-;"
    "_,q1->_,q2,->"
  ;

  struct turing* tm = tm_create_machine(def);

  return 0;
}
