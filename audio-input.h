#ifndef AUDIO_INPUT_H
#define AUDIO_INPUT_H

#include <cstdint>
#include <cstdlib>
#include <sphinxbase/ad.h>
#include <sphinxbase/err.h>

namespace oas {

struct AudioInput {
  enum State {
    kNoRecording,
    kRecording,
  };

  struct Chunk {
    const int16_t * pointer;
    std::size_t size;

    Chunk(void) : pointer(NULL), size(0) { }
  };

  ad_rec_t * const input_;
  const std::size_t size_;
  int16_t * const buffer_;
  State state_;

  AudioInput(const char * const f = NULL, const std::size_t s = 2048);

  void startRecording(void);
  void stopRecording(void);
  Chunk read(void);
  State state(void) const;
};

} //end of oas namespace

#endif //AUDIO_INPUT_H
