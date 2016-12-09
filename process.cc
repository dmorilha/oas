#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/wait.h>

#include "process.h"

namespace oas {

Process::~Process(void) {
  if (0 != pid_) {
    assert(0 != in_);
    assert(0 != out_);
    assert(0 != error_);
    close(in_);
    close(out_);
    close(error_);
  }
}

Process::Process(void) : pid_(0),
  in_(0), out_(0), error_(0) { }

bool Process::execute(const char * const e, const char * const * a) {
  if (0 != pid_) { return false; }

  int pipes[6];

  pipe(pipes);
  pipe(pipes + 2);
  pipe(pipes + 4);

  pid_ = fork();

  // Child
  if (0 == pid_) {

    dup2(pipes[0], STDIN_FILENO);

    /*
       dup2(pipes[3], STDOUT_FILENO);
       dup2(pipes[5], STDERR_FILENO);
     */

    for (int i = 0; i < 6; ++i) {
      close(pipes[i]);
    }

    execv(e, const_cast< char * const * >(a));

    /*
     * Nothing below this line should be executed by child process. If so,
     * it means that the execl function wasn't successfull, so lets exit:
     */
    assert(false);
    return false;
  }

  close(pipes[0]);
  in_ = pipes[1];
  out_ = pipes[2];
  close(pipes[3]);
  error_ = pipes[4];
  close(pipes[5]);

  return true;
}

void Process::write(const char * const s) {
  assert(NULL != s);
  if (0 == pid_) { return; }
  assert(0 != in_);
  const size_t length = strlen(s);
  const int written = ::write(in_, s, length);
  assert(length == static_cast< size_t >(written));
}

bool Process::kill(void) {
  //TODO(dmorilha): not working...
  if (0 == pid_) { return false; }
  if ( ! exists()) { return false; }
  const bool result = ::kill(pid_, SIGQUIT) == 0;
  this->~Process();
  pid_ = 0;
  return result;
}

bool Process::exists(void) const {
  if (0 == pid_) { return false; }
  if (::kill(pid_, 0) != 0) {
    return errno != ESRCH;
  }
  return true;
}
} //end of oas namespace
