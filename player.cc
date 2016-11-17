#include <assert.h>

#include "player.h"

namespace oas {

Player::Player(void) : process_(NULL) { }

void Player::play(const char * const v) {
  if (NULL != process_) {
    disposeProcess();
  }

  process_ = new Process();

  static const char * const PLAYER = "/usr/bin/omxplayer";

  const char * const arguments[] = {
    PLAYER,
    v,
    NULL
  };

  process_->execute(PLAYER, arguments);
}

void Player::stop(void) {
  if (NULL == process_) { return; }
  process_->write("q");
  disposeProcess();
}

void Player::disposeProcess(void) {
  assert(NULL != process_);
  process_->kill();
  delete process_;
  process_ = NULL;
}

} //end of oas namespace
