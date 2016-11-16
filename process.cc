#include <assert.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "process.h"

namespace oas {

Process::Process(void) : pid_(0) { }

bool Process::execute(const char * const e, const char * const * a) {
  if (0 != pid_) { return false; }

  pipe(pipes_);
  pipe(pipes_ + 2);

  close(pipes_[2]);

  pid_ = fork();

  if (0 == pid_) {
    // Child
    dup2(pipes_[0], STDIN_FILENO);

    /*
    dup2(pipes_[1], STDOUT_FILENO);
    dup2(pipes_[3], STDERR_FILENO);
    */

    execv(e, const_cast< char * const * >(a));

    /*
     * Nothing below this line should be executed by child process. If so,
     * it means that the execl function wasn't successfull, so lets exit:
     */
    assert(false);
    return false;
  }

  return true;
}



} //end of oas namespace
