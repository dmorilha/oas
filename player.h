#ifndef PLAYER_H
#define PLAYER_H

#include "process.h"

namespace oas {

struct Player {
  Process process_;
  void play(const char * const);
};

} //end of oas namespace

#endif //PLAYER_H
