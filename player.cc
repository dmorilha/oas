#include <assert.h>

#include "player.h"

namespace oas {

void disposeProcess(Process * & p) {
  assert(NULL != p);
  p->kill();
  delete p;
  p = NULL;
}

void execute(Process * & p, const char * const v) {
  assert(NULL != v);

  if (NULL != p) {
    disposeProcess(p);
  }

  assert(NULL == p);

  p = new Process();

  static const char * const PLAYER = "/usr/bin/omxplayer";

  const char * const arguments[] = {
    PLAYER,
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

Player::Player(void) : preloadedProcess_(NULL), process_(NULL) { }

void Player::play(const char * const v) {
  execute(process_, v);
}

void Player::preload(const char * const v) {
  execute(preloadedProcess_, v);
  assert(NULL != preloadedProcess_);
  preloadedProcess_->write("p"); //pause
}

void Player::stop(void) {
  if (NULL == process_) { return; }
  process_->write("q");
  disposeProcess(process_);
  //TODO(dmorilha): should this be only on next?
  if (NULL != preloadedProcess_) {
    process_ = preloadedProcess_;
    preloadedProcess_ = NULL;
    process_->write("p"); //resume
  }
}

void Player::pause(void) {
  if (NULL == process_) { return; }
  process_->write("p"); //pause
}

} //end of oas namespace
