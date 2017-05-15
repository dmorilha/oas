#include <assert.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "audio-input.h"
#include "controller.h"
#include "dbus-handler.h"
#include "hs200.h"
#include "media.h"
#include "player.h"
#include "queue.h"
#include "sphinx.h"
#include "tv.h"

static const int FIVE_MINUTES = 60 * 5 * 2;
static const int TEN_MINUTES = 60 * 10 * 2;
static const int ONE_MINUTE = 60 * 2;

oas::Player player;
oas::Queue queue;
oas::TV tv;
oas::HS200 lights("192.168.1.67");

oas::Controller controller(&player, &queue, &tv, &lights);

oas::DBUS dbus(&controller);
//oas::Sphinx sphinx(&controller);

void loop(void) {
  using namespace oas;

  std::string string;

  int secondsToStandBy = FIVE_MINUTES;
  int counter1 = 0;
  int interval = 500000;

  while (true) {
    const Player::State playerState = player.state();
    const TV::State tvState = tv.state();
    const Media * const media = player.media();
    Media::Type mediaType = Media::Type::kUndefined;
    if (NULL != media) {
      mediaType = media->type();
    }
    const bool isAudio = Media::kAudio == mediaType;

      /*
    {
      AudioInput * const input = sphinx.input();
      assert(NULL != input);
      const AudioInput::State inputState = input->state();
      if (Player::kPlaying == playerState) {
        if (AudioInput::kRecording == inputState) {
          input->stopRecording();
        }
        interval = 500000;
      } else {
        if (AudioInput::kNoRecording == inputState) {
          input->startRecording();
        }
        sphinx.processVoice();
        interval = 100000;
      }
    }
         */


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

    } else if (Player::kPlaying != playerState || isAudio) {
      if (TV::kStandby != tvState) {
        --secondsToStandBy;
        if (0 == secondsToStandBy) {
          tv.standby();
          std::cout << "standby " << std::endl;
        } else if (0 == secondsToStandBy % ONE_MINUTE) {
          std::cout << (secondsToStandBy / 2)
            << " to go to standby" << std::endl;
        }
      }


    } else {
      //There is a problem is tv.on returns false.
      const Media * const media = player.media();
      assert(NULL != media);
      if (TV::kOn != tvState && ! isAudio) {
        {
          const TriValue result = tv.pingAdapter();
          std::cout << "ping " << print(result) << std::endl;
        }
        {
          const TriValue result = tv.on();
          std::cout << "on " << print(result) << std::endl;
        }
        {
          const TriValue result = tv.setActiveSource();
          std::cout << "set active source " << print(result) << std::endl;
        }
      }
      secondsToStandBy = FIVE_MINUTES;
    }

    dbus.processMessages();

    //TODO(dmorilha): replace this with network select and same timeout.
    usleep(interval);
  }
}

int main(void) {
  signal(SIGCHLD, SIG_IGN);
  dbus.listen();
  player.setAudioDevice(oas::AudioDevice::kBoth);
  loop();
  return 0;
}
