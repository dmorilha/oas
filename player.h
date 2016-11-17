#ifndef PLAYER_H
#define PLAYER_H

#include "process.h"

namespace oas {

struct Player {
  Process * preloadedProcess_, * process_;
  ~Player(void);
  Player(void);
  void play(const char * const);
  void preload(const char * const);
  void pause(void);
  void stop(void);
};

} //end of oas namespace

#endif //PLAYER_H
