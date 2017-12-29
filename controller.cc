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

void Controller::forward(const int s) {
  std::cout << "forward " << s << " seconds" << std::endl;
  assert(NULL != player_);
  player_->forward(s);
}

void Controller::repeat(void) {
  std::cout << "repeat" << std::endl;
  assert(NULL != player_);
  player_->repeat();
}

void Controller::resume(void) {
  std::cout << "resume" << std::endl;
  assert(NULL != player_);
  player_->resume();
}

void Controller::rewind(const int s) {
  std::cout << "rewind " << s << " seconds" << std::endl;
  assert(NULL != player_);
  player_->rewind(s);
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

void Controller::setAudioDevice(const AudioDevice::Device a) {
  assert(NULL != player_);
  std::cout << "set audio to " << AudioDevice::print(a) << std::endl;
  player_->setAudioDevice(a);
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

void Controller::pressKey(const char * const s) {
  assert(NULL != player_);
  std::cout << "press key(s) \"" << s << "\"" <<  std::endl;
  player_->pressKey(s);
}
} //end of oas namespace
