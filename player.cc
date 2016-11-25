#include <assert.h>
#include <iostream>

#include "player.h"

namespace oas {

void disposeProcess(Process * & p) {
  assert(NULL != p);
  p->kill();
  delete p;
  p = NULL;
}

void execute(Process * & p, const char * const v, const int vo = 0) {
  assert(NULL != v);

  if (NULL != p) {
    disposeProcess(p);
  }

  assert(NULL == p);

  p = new Process();

  static const char * const PLAYER = "/usr/bin/omxplayer";
  char volume[64] = { 0 };
  sprintf(volume, "%d", vo * 300);
  std::cout << "volume " << volume << std::endl;

  const char * const arguments[] = {
    PLAYER,
    "--blank",
    "--with-info",
    "--adev", "both",
    "--refresh",
    "--audio_queue", "1",
    "--threshold", "10",
    "--timeout", "60",
    "--video_queue", "2",
    "--font", "/usr/share/fonts/TTF/DejaVuSans.ttf",
    "--italic-font", "/usr/share/fonts/TTF/DejaVuSans-Oblique.ttf",
    "--vol", volume,
    v,
    NULL
  };

  p->execute(PLAYER, arguments);
}

Player::~Player(void) {
  if (NULL != preloadedProcess_) {
    disposeProcess(preloadedProcess_);
  }

  if (NULL != process_) {
    disposeProcess(process_);
  }
}

Player::Player(void) : state_(kStopped), preloadedProcess_(NULL), process_(NULL),
  volume_(0) { }

void Player::play(const char * const v) {
  execute(process_, v, volume_);
  if ( ! process_->exists()) {
    disposeProcess(process_);
  } else {
    state_ = kPlaying;
  }
}

void Player::preload(const char * const v) {
  execute(preloadedProcess_, v, volume_);
  assert(NULL != preloadedProcess_);
  preloadedProcess_->write("p"); //pause
}

void Player::stop(void) {
  if (NULL == process_) { return; }
  if (kStopped == state_) { return; }
  process_->write("q");
  disposeProcess(process_);
  state_ = kStopped;

  //TODO(dmorilha): should this be only on next?
  if (NULL != preloadedProcess_) {
    process_ = preloadedProcess_;
    preloadedProcess_ = NULL;
    process_->write("p"); //resume
  }
}

void Player::pause(void) {
  if (NULL == process_) { return; }
  if (kPlaying != state_) { return; }
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
    case kPlaying:
      if ( ! process_->exists()) {
        state_ = kStopped;
      } else {
        return state_;
      }
      break;
    default:
      break;
    }
  }
  return kStopped;
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

} //end of oas namespace
