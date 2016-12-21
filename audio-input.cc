#include <assert.h>

#include "audio-input.h"

namespace oas {

AudioInput::~AudioInput() {
  assert(NULL != buffer_);
  free(buffer_);
  if (NULL != input_) {
    ad_close(input_);
  }
}

AudioInput::AudioInput(const char * const d, const std::size_t s) :
  input_(ad_open_dev(d, 0)),
  size_(s),
  buffer_(reinterpret_cast< int16_t * >(malloc(size_ * sizeof(int16_t)))) {
    assert(128 <= size_);
  }

void AudioInput::startRecording(void) {
  if (NULL != input_) {
    const int r = ad_start_rec(input_);
    assert(0 < r);
  }
}

void AudioInput::stopRecording(void) {
  if (NULL != input_) {
    const int r = ad_stop_rec(input_);
    assert(0 < r);
  }
}

AudioInput::Chunk AudioInput::read(void) {
  AudioInput::Chunk chunk;
  if (NULL != input_) {
    chunk.size = ad_read(input_, buffer_, size_);
    chunk.pointer = buffer_;
  }
  return chunk;
}

} //end of oas namespace
