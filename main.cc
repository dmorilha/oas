#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include "dbus.h"
#include "player.h"
#include "queue.h"
#include "tv.h"

static const int FIVE_MINUTES = 60 * 5 * 2;

oas::Player player;
oas::DBUS dbus(&player);

void loop(void) {
  using namespace oas;

  Queue queue;
  TV tv;
  std::string string;

  int secondsToStandBy = FIVE_MINUTES;

  while (true) {
    const TV::State tvState = tv.state();
    if (Player::kPlaying != player.state()) {
      if (TV::kStandby != tvState
          && 0 == --secondsToStandBy) {
        const bool result = tv.standby();
        std::cout << "standby " << (result ? "true" : "false") << std::endl;
      }
    } else {
      if (TV::kOn != tvState) {
        {
          const bool result = tv.on();
          std::cout << "on " << (result ? "true" : "false") << std::endl;
        }
        {
          const bool result = tv.setActiveSource();
          std::cout << "set active source " << (result ? "true" : "false") << std::endl;
        }
      }
      secondsToStandBy = FIVE_MINUTES;
    }

    dbus.processMessages();

    usleep(500000);
  }
}

int main(void) {
  std::cout << "omxplayer-dbus" << std::endl;
  dbus.listen();
  loop();
  return 0;
}
