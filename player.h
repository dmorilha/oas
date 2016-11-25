#ifndef PLAYER_H
#define PLAYER_H

#include "process.h"

namespace oas {

struct Player {
  enum State {
    kStopped,
    kPlaying,
    kPaused,
  };

  State state_;

  Process * preloadedProcess_, * process_;

  int volume_;

  ~Player(void);
  Player(void);

  void pause(void);
  void play(const char * const);
  void preload(const char * const);
  void resume(void);
  void stop(void);
  void volumeUp(void);
  void volumeDown(void);
  State state(void);
};

} //end of oas namespace

#endif //PLAYER_H
