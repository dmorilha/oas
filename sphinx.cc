#include <assert.h>
#include <iostream>

#include "audio-input.h"
#include "controller.h"
#include "sphinx.h"

namespace oas {
Sphinx::~Sphinx() {
  assert(NULL != decoder_);
  ps_free(decoder_);
}

Sphinx::Sphinx(Controller * const c) :
  decoder_(NULL), controller_(c),
  input_(new AudioInput), started_(false) {
  assert(NULL != controller_);
  assert(NULL != input_);

  cmd_ln_t * const config = cmd_ln_init(NULL, ps_args(), TRUE,
      "-hmm", MODELDIR "/en-us/en-us",
      //"-lm", MODELDIR "/en-us/en-us.lm.bin",
      "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
      "-kws", "./oas.gram",

      NULL);
  const_cast< ps_decoder_t * & >(decoder_) = ps_init(config); 
  assert(NULL != decoder_);
}

void Sphinx::dispatch(const char * const s) {
  assert(NULL != controller_);

#define SPHINX_DISPATCH(P, M) \
  if (strcmp(#P, s) == 0) { \
    controller_->M(); \
  } else

  SPHINX_LIST(SPHINX_DISPATCH)

  // else
  {
    std::cerr << "keyword \"" << s << "\" not recognized." << std::endl;
  }
}

void Sphinx::processVoice(void) {
  assert(NULL != input_);
  AudioInput::Chunk chunk = input_->read();

  assert(NULL != decoder_);
  ps_process_raw(decoder_, chunk.pointer, chunk.size, 0, 0);

  const bool inSpeech = ps_get_in_speech(decoder_);

  if (inSpeech && ! started_) {
    started_ = true;
  }

  if ( ! inSpeech && started_) {
    /* speech -> silence transition, time to start new utterance  */
    ps_end_utt(decoder_);
    const char * const result = ps_get_hyp(decoder_, NULL);

    if (NULL != result && 0 < strlen(result)) {
      std::cout << "recognized " << result << std::endl;
      dispatch(result);
    }

    ps_start_utt(decoder_);
    started_ = false;
  }
}

void Sphinx::startRecording(void) {
  assert(NULL != input_);
  input_->startRecording();
}

void Sphinx::stopRecording(void) {
  assert(NULL != input_);
  input_->stopRecording();
}

} //end of oas namespace
