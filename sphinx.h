#ifndef SPHINX_H
#define SPHINX_H

#include <pocketsphinx.h>

namespace oas {

struct AudioInput;
struct Controller;

#define SPHINX_LIST(V) \
  V(bitch please, lightsOff) \
  V(hello, lightsOn) \
  V(lights off, lightsOff) \
  V(lights on, lightsOn) \
  V(lights up, lightsOn) \
  V(next, next) \
  V(repeat, repeat) \
  V(resume, resume) \
  V(turn the lights on, lightsOn) \
  V(tv off, tvOff) \
  V(t v off, tvOff) \
  V(tv on, tvOn) \
  V(t v on, tvOn) \
  V(tv up, tvOn) \
  V(t v up, tvOn) \

struct Sphinx {
  ps_decoder_t * const decoder_;
  Controller * const controller_;
  AudioInput * const input_;
  bool started_;

  ~Sphinx(void);
  Sphinx(Controller * const);

  void dispatch(const char * const);
  void processVoice(void);
  AudioInput * input(void);
};
} //end of oas namespace
#endif //SPHINX_H
