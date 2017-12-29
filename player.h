#ifndef PLAYER_H
#define PLAYER_H

#include "audio-device.h"
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
  Media * media_;
  int volume_;
  AudioDevice::Device audioDevice_;

  ~Player(void);
  Player(void);

  State state(void);
  Media * media(void) const;

  void end(void);
  void forward(const int) const;
  void pause(void);
  void play(const Media &);
  void pressKey(const char *) const;
  void repeat(void);
  void resume(void);
  void rewind(const int) const;
  void stop(void);
  void volumeDown(void);
  void volumeUp(void);
  inline void setAudioDevice(const AudioDevice::Device a) { audioDevice_ = a; }
};

} //end of oas namespace

#endif //PLAYER_H
