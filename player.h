#ifndef PLAYER_H
#define PLAYER_H

#include "media.h"
#include "process.h"

namespace oas {

/*

## KEY BINDINGS

Key bindings to control omxplayer while playing:

    1           decrease speed
    2           increase speed
    <           rewind
    >           fast forward
    z           show info
    j           previous audio stream
    k           next audio stream
    i           previous chapter
    o           next chapter
    n           previous subtitle stream
    m           next subtitle stream
    s           toggle subtitles
    w           show subtitles
    x           hide subtitles
    d           decrease subtitle delay (- 250 ms)
    f           increase subtitle delay (+ 250 ms)
    q           exit omxplayer
    p / space   pause/resume
    -           decrease volume
    + / =       increase volume
    left arrow  seek -30 seconds
    right arrow seek +30 seconds
    down arrow  seek -600 seconds
    up arrow    seek +600 seconds

 */

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

  void chapterNext(void) const;
  void chapterPrevious(void) const;
  void end(void);
  void forward30(void) const;
  void forward600(void) const;
  void pause(void);
  void play(const Media &);
  void resume(void);
  void rewind30(void) const;
  void rewind600(void) const;
  void showInfo(void) const;
  void stop(void);
  void volumeDown(void);
  void volumeUp(void);
  void nextSubtitleStream(void) const;

  inline void setBluetooth(const bool v) {
    bluetooth_ = v;
  }
};

} //end of oas namespace

#endif //PLAYER_H
