#ifndef PROCESS_H
#define PROCESS_H

#include <unistd.h>

namespace oas {

struct Stream;

struct Process {
  pid_t pid_;
  int in_, out_, error_;

  ~Process();
  Process(void);

  bool execute(const char * const, const char * const *);
  bool kill(void);
  void wait(void) const;
  void write(const char * const);
  bool exists(void) const;
};

} //end of oas namespace

#endif //PROCESS_H
