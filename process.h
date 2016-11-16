#ifndef PROCESS_H
#define PROCESS_H

#include <unistd.h>

namespace oas {

struct Process {
  int pipes_[4];
  pid_t pid_;

  Process(void);

  bool execute(const char * const, const char * const *);
  void wait(void) const;
};

} //end of oas namespace

#endif //PROCESS_H
