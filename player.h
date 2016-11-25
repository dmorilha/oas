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

  State state(void);
  void forward30(void) const;
  void forward600(void) const;
  void pause(void);
  void play(const char * const);
  void preload(const char * const);
  void resume(void);
  void rewind30(void) const;
  void rewind600(void) const;
  void stop(void);
  void volumeDown(void);
  void volumeUp(void);
};

} //end of oas namespace

#endif //PLAYER_H
