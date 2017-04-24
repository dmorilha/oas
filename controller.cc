#include <assert.h>
#include <iostream>

#include "controller.h"
#include "lights.h"
#include "player.h"
#include "queue.h"
#include "tv.h"

namespace oas {

Controller::Controller(Player * const p, Queue * const q, TV * const t,
    Lights * const l) : player_(p), queue_(q), tv_(t), lights_(l) {
  assert(NULL != player_);
  assert(NULL != queue_);
}

void Controller::clear(void) {
  std::cout << "clear" << std::endl;
  assert(NULL != queue_);
  queue_->clear();
}

void Controller::next(void) {
  std::cout << "next" << std::endl;
  assert(NULL != queue_);
  if ( ! queue_->empty()) {
    assert(NULL != player_);
    player_->end();
  }
}

void Controller::play(const Media & m) {
  assert(NULL != player_);
  player_->play(m);
}

void Controller::previous(void) { }

void Controller::pushBack(const Media & m) {
  assert(NULL != queue_);
  assert(NULL != player_);
  if (Player::kStopped == player_->state() && queue_->empty()) {
    player_->play(m);
  } else {
    queue_->pushBack(m);
  }
}

void Controller::pushFront(const Media & m) {
  assert(NULL != queue_);
  assert(NULL != player_);
  if (Player::kStopped == player_->state() && queue_->empty()) {
    player_->play(m);
  } else {
    queue_->pushFront(m);
  }
}

void Controller::stop(void) {
  std::cout << "stop" << std::endl;
  assert(NULL != player_);
  player_->stop();
}

void Controller::volumeDown(void) {
  std::cout << "volume down" << std::endl;
  assert(NULL != player_);
  player_->volumeDown();
}

void Controller::volumeUp(void) {
  std::cout << "volume up" << std::endl;
  assert(NULL != player_);
  player_->volumeUp();
}

void Controller::forward30(void) {
  std::cout << "forward 30 seconds" << std::endl;
  assert(NULL != player_);
  player_->forward30();
}

void Controller::forward600(void) {
  std::cout << "forward 600 seconds" << std::endl;
  assert(NULL != player_);
  player_->forward600();
}

void Controller::repeat(void) {
  std::cout << "repeat" << std::endl;
  assert(NULL != player_);
  Media * const media = player_->media();
  if (NULL != media) {
    media->repeat();
  }
}

void Controller::resume(void) {
  std::cout << "resume" << std::endl;
  assert(NULL != player_);
  player_->resume();
}

void Controller::rewind30(void) {
  std::cout << "rewind 30 seconds" << std::endl;
  assert(NULL != player_);
  player_->rewind30();
}

void Controller::rewind600(void) {
  std::cout << "rewind 600 seconds" << std::endl;
  assert(NULL != player_);
  player_->rewind600();
}

void Controller::pause(void) {
  std::cout << "pause" << std::endl;
  assert(NULL != player_);
  player_->pause();
}

void Controller::tvOn(void) {
  if (NULL != tv_) {
    std::cout << "turn on" << std::endl;
    tv_->on();
  }
}

void Controller::tvOff(void) {
  if (NULL != tv_) {
    std::cout << "tv off" << std::endl;
    tv_->standby();
  }
}

void Controller::lightsOn(void) {
  if (NULL != lights_) {
    std::cout << "lights on" << std::endl;
    lights_->on();
  }
}

void Controller::lightsOff(void) {
  if (NULL != lights_) {
    std::cout << "lights off" << std::endl;
    lights_->off();
  }
}

void Controller::bluetoothOn(void) {
  assert(NULL != player_);
  std::cout << "bluetooth on" << std::endl;
  player_->setBluetooth(true);
}

void Controller::bluetoothOff(void) {
  assert(NULL != player_);
  std::cout << "bluetooth off" << std::endl;
  player_->setBluetooth(false);
}

void Controller::chapterNext(void) {
  assert(NULL != player_);
  std::cout << "chapter next" << std::endl;
  player_->chapterNext();
}

void Controller::chapterPrevious(void) {
  assert(NULL != player_);
  std::cout << "chapter previous" << std::endl;
  player_->chapterPrevious();
}

void Controller::showInfo(void) {
  assert(NULL != player_);
  std::cout << "show info" << std::endl;
  player_->showInfo();
}

void Controller::nextSubtitleStream(void) {
  assert(NULL != player_);
  std::cout << "next subtitle stream" << std::endl;
  player_->nextSubtitleStream();
}

void Controller::speedIncrease(void) {
  assert(NULL != player_);
  std::cout << "speed increase" << std::endl;
  player_->speedIncrease();
}

void Controller::speedDecrease(void) {
  assert(NULL != player_);
  std::cout << "speed decrease" << std::endl;
  player_->speedDecrease();
}
} //end of oas namespace
