#include <assert.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "dbus.h"
#include "player.h"
#include "queue.h"
#include "tv.h"

static const int FIVE_MINUTES = 60 * 5 * 2;
static const int TEN_MINUTES = 60 * 10 * 2;
static const int ONE_MINUTES = 60 * 2;

oas::Player player;
oas::Queue queue;
oas::TV tv;

oas::DBUS dbus(&player, &queue, &tv);

void loop(void) {
  using namespace oas;

  std::string string;

  int secondsToStandBy = FIVE_MINUTES;
  int counter1 = 0;

  while (true) {

    const Player::State playerState = player.state();
    const TV::State tvState = tv.state();

    /*
    if (counter1++ % ONE_MINUTES == 0) {
      const TriValue result = tv.pingAdapter();
      std::cout << "ping returned " << print(result) << std::endl
        << "player state is " << Player::print(playerState) << std::endl
        << "tv state is " << TV::print(tvState) << std::endl
        << std::endl;
    }
    */

    if (Player::kEnded == playerState
        && ! queue.empty()) {
      Media m;
      queue.next(m);
      player.play(m);
      std::cout << "playing next item " << m.location() << std::endl;

    } else if (Player::kPlaying != playerState) {
      if (TV::kStandby != tvState
          && 0 == --secondsToStandBy) {
        const bool result = tv.standby();
        std::cout << "standby " << (result ? "true" : "false") << std::endl;
      }
    } else {
      //There is a problem is tv.on returns false.
      const Media * const media = player.media();
      assert(NULL != media);
      if (TV::kOn != tvState && media->type() != Media::kAudio) {
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
  signal(SIGCHLD, SIG_IGN);
  dbus.listen();
  loop();
  return 0;
}
