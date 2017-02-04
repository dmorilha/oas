#ifndef PLAYER_H
#define PLAYER_H

#include "media.h"
#include "process.h"

namespace oas {

struct Player {
  enum State {
    kUnknown,
    kCrashed,
    kEnded,
    kPaused,
    kPlaying,
    kStopped,
  };

  static const char * print(const State);

  State state_;

  Process * process_;
  //dbus::Connection * dbus_;
  Media * media_;
  int volume_;
  bool bluetooth_;

  ~Player(void);
  Player(void);

  State state(void);
  Media * media(void) const;

  void end(void);
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

  inline void setBluetooth(const bool v) {
    bluetooth_ = v;
  }
};

} //end of oas namespace

#endif //PLAYER_H
