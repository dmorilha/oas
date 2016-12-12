#ifndef PLAYER_H
#define PLAYER_H

#include "media.h"
#include "process.h"

namespace oas {

struct Player {
  enum State {
    kUnknown,
    kEnded,
    kPaused,
    kPlaying,
    kStopped,
  };

  static const char * print(const State);

  State state_;

  Process * process_;
  Media * media_;

  int volume_;

  ~Player(void);
  Player(void);

  State state(void);
  const Media * media(void) const;

  void forward30(void) const;
  void forward600(void) const;
  void pause(void);
  void play(const Media &);
  void resume(void);
  void rewind30(void) const;
  void rewind600(void) const;
  void stop(void);
  void volumeDown(void);
  void volumeUp(void);
};

} //end of oas namespace

#endif //PLAYER_H
