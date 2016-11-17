#ifndef PLAYER_H
#define PLAYER_H

#include "process.h"

namespace oas {

struct Player {
  Process * process_;
  Player(void);
  void play(const char * const);
  void stop(void);
  void disposeProcess(void);
};

} //end of oas namespace

#endif //PLAYER_H
