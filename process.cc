#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/wait.h>

#include "process.h"

namespace oas {

/*
struct Stream {
  FILE * steam_;

  ~Stream() {
    assert(NULL != stream_);
    fclose(stream_);
  }

  Stream(const int f, const char const * m) {
    assert(0 != f);
    stream_ = fdopen(f, m);
    assert(NULL != stream_);
  }

  void write(const char * const s) {
    assert(stream_ != NULL);
    fputs(s, stream_);
  }
};
*/

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
  if (-1 == written) {
    if (0 != errno) {
      perror(NULL);
    }
  } else {
    assert(length == written);
  }
  std::cerr << "len " << length << " " << written << std::endl;
}

bool Process::kill(void) {
  if (0 != pid_) { return false; }
  return ::kill(pid_, SIGQUIT) == 0;

}

} //end of oas namespace
