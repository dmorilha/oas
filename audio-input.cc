#include <assert.h>

#include "audio-input.h"

namespace oas {

AudioInput::AudioInput(const char * const f, const std::size_t s) :
  input_(ad_open_dev(f, 0)),
  size_(s),
  buffer_(reinterpret_cast< int16_t * >(malloc(size_ * sizeof(int16_t)))),
  state_(kNoRecording) {
    assert(128 <= size_);
  }

void AudioInput::startRecording(void) {
  if (NULL != input_) {
    assert(kNoRecording == state_);
    const int r = ad_start_rec(input_);
    assert(0 == r);
    state_ = kRecording;
  }
}

void AudioInput::stopRecording(void) {
  if (NULL != input_) {
    assert(kRecording == state_);
    const int r = ad_stop_rec(input_);
    assert(0 == r);
    state_ = kNoRecording;
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

AudioInput::State AudioInput::state(void) const {
  return state_;
}

} //end of oas namespace
