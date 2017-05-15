#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "audio-device.h"
#include "media.h"

namespace oas {
struct Lights;
struct Player;
struct Queue;
struct TV;

struct Controller {
  Player * const player_;
  Queue * const queue_;
  TV * const tv_;
  Lights * const lights_;

  Controller(Player * const, Queue * const, TV * const t = NULL,
       Lights * const l = NULL);

  void chapterNext(void);
  void chapterPrevious(void);
  void clear(void);
  void forward(const int);
  void lightsOff(void);
  void lightsOn(void);
  void next(void);
  void pause(void);
  void play(const Media &);
  void previous(void);
  void pushBack(const Media &);
  void pushFront(const Media &);
  void repeat(void);
  void resume(void);
  void rewind(const int);
  void setAudioDevice(const AudioDevice::Device);
  void showInfo(void);
  void stop(void);
  void tvOff(void);
  void tvOn(void);
  void volumeDown(void);
  void volumeUp(void);
  void nextSubtitleStream(void);
  void speedIncrease(void);
  void speedDecrease(void);
};
} //end of oas namespace

#endif //CONTROLLER_H
