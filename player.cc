#include "player.h"

namespace oas {

void Player::play(const char * const v) {
  static const char * const PLAYER = "/usr/bin/omxplayer";
  const char * const arguments[] = {PLAYER, v, NULL};
  process_.execute(PLAYER, arguments);
}

} //end of oas namespace
