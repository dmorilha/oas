#ifndef SPHINX_H
#define SPHINX_H

#include <pocketsphinx.h>

namespace oas {

struct AudioInput;
struct Controller;

#define SPHINX_LIST(V) \
  V(lights on, lightsOn) \
  V(lights off, lightsOff) \
  V(next, next) \
  V(repeat, repeat) \
  V(resume, resume) \
  V(tv on, tvOn) \
  V(tv off, tvOff) \

struct Sphinx {
  ps_decoder_t * const decoder_;
  Controller * const controller_;
  AudioInput * const input_;
  bool started_;

  ~Sphinx(void);
  Sphinx(Controller * const);

  void dispatch(const char * const);
  void processVoice(void);
  void startRecording(void);
  void stopRecording(void);
};
} //end of oas namespace
#endif //SPHINX_H
