#include <assert.h>
#include <iostream>

#include "player.h"

namespace oas {

const char * Player::print(const State s) {
  switch (s) {
  case kCrashed:
    return "crashed";
  case kEnded:
    return "ended";
  case kPaused:
    return "paused";
  case kPlaying:
    return "playing";
  case kStopped:
    return "stopped";
  case kUnknown:
  default:
    return "unknown";
  }
}

void disposeProcess(Process * & p) {
  assert(NULL != p);
  //p->kill();
  delete p;
  p = NULL;
}

void execute(Process * & p, const char * const v, const int vo = 0, const bool b = false) {
  assert(NULL != v);

  if (NULL != p) {
    disposeProcess(p);
  }

  assert(NULL == p);

  p = new Process();

  char volume[64] = { 0 };
  sprintf(volume, "%d", vo * 300);

  static const char * const PLAYER = "/usr/bin/omxplayer";
  const char * const arguments[] = {
    PLAYER,
    "--blank",
    "--with-info",
    "--adev", (b ? "alsa" : "both"),
    "--refresh",
    "--font", "/usr/share/fonts/TTF/DejaVuSans.ttf",
    "--italic-font", "/usr/share/fonts/TTF/DejaVuSans-Oblique.ttf",
    "--vol", volume,
    v,
    NULL
  };

  p->execute(PLAYER, arguments);
}

Player::~Player(void) {
  if (NULL != process_) {
    disposeProcess(process_);
  }

  if (NULL != media_) {
    delete media_;
  }
}

Player::Player(void) : state_(kEnded), process_(NULL),
  media_(NULL), volume_(0), bluetooth_(false) { }

void Player::play(const Media & m) {
  const State s = state();
  if (kPlaying == s || kPaused == s) {
    assert(NULL != process_);
    stop();
  }

  //addressing repeat
  if (&m != media_) {
    assert(NULL != m.location());
    if (NULL != media_) {
      delete media_;
    }
    media_ = new Media(m);
  }

  execute(process_, media_->location(), volume_, bluetooth_);

  if ( ! process_->exists()) {
    disposeProcess(process_);
  } else {
    state_ = kPlaying;
  }
}

void Player::end(void) {
  if (NULL != process_) {
    process_->write("q");
    disposeProcess(process_);
  }
  state_ = kEnded;
}

void Player::stop(void) {
  const State s = state();
  if (NULL == process_ || kStopped == s || kEnded == s) {
    return;
  }
  process_->write("q");
  state_ = kStopped;
  disposeProcess(process_);
}

void Player::pause(void) {
  if (NULL == process_) { return; }
  if (kPlaying != state()) { return; }
  process_->write("p"); //pause
  state_ = kPaused;
}

void Player::resume(void) {
  if (NULL == process_) { return; }
  if (kPaused != state_) { return; }
  process_->write("p"); //pause
  state_ = kPlaying;
}

Player::State Player::state(void) {
  if (NULL != process_) {
    switch (state_) {
    case kPaused:
      if ( ! process_->exists()) {
        state_ = kCrashed;
      }
      break;

    case kPlaying:
      if ( ! process_->exists()) {
        state_ = kEnded;
        assert(NULL != media_);
        const int repeat = media_->repeating();
        if (0 < repeat) {
          std::cout << "repeating " << repeat << std::endl;
          play(*media_);
        }
      }
      break;

    default:
      break;
    }
  }
  return state_;
}

Media * Player::media(void) const {
  return media_;
}

void Player::volumeUp(void) {
  ++volume_;
  if (NULL == process_) { return; }
  process_->write("+"); //volume up
}

void Player::volumeDown(void) {
  --volume_;
  if (NULL == process_) { return; }
  process_->write("-"); //volume down
}

void Player::forward30(void) const {
  if (NULL == process_) { return; }
  process_->write("\x1b\x5b\x43"); //forward 30 seconds
}

void Player::forward600(void) const {
  if (NULL == process_) { return; }
  process_->write("\x1b\x5b\x41"); //forward 600 seconds
}

void Player::rewind30(void) const {
  if (NULL == process_) { return; }
  process_->write("\x1b\x5b\x44"); //rewind 30 seconds
}

void Player::rewind600(void) const {
  if (NULL == process_) { return; }
  process_->write("\x1b\x5b\x42"); //rewind 600 seconds
}

void Player::chapterNext(void) const {
  if (NULL == process_) { return; }
  process_->write("o"); //next chapter
}

void Player::chapterPrevious(void) const {
  if (NULL == process_) { return; }
  process_->write("i"); //previous chapter
}

void Player::showInfo(void) const {
  if (NULL == process_) { return; }
  process_->write("z"); //show info
}

void Player::nextSubtitleStream(void) const {
  if (NULL == process_) { return; }
  process_->write("m"); //next subtitle stream
}

} //end of oas namespace
