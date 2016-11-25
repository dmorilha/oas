#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include "dbus.h"
#include "player.h"
#include "queue.h"
#include "tv.h"

static const int TEN_MINUTES = 60 * 10 * 10;

oas::Player player;
oas::DBUS dbus(&player);

void loop(void) {
  using namespace oas;

  Queue queue;
  TV tv;
  std::string string;
  tv.on();

  int secondsToTurnOff = TEN_MINUTES;

  while (true) {
    if (Player::kPlaying != player.state()) {
      if (0 == --secondsToTurnOff) {
        tv.standby();
      }
    } else {
      secondsToTurnOff = TEN_MINUTES;
    }

    dbus.processMessages();

    sleep(1);
  }
}

int main(void) {
  std::cout << "omxplayer-dbus" << std::endl;
  dbus.listen();
  loop();
  return 0;
}
