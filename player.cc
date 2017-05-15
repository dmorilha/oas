#include <assert.h>
#include <iostream>
#include <vector>

#include "player.h"

namespace oas {

const char * Player::print(const State s) {
  switch (s) {
  case kCrashed:
    return "crashed";
  case kEnded:
    return "ended";
  case kPaused:
    return "paused";
  case kPlaying:
    return "playing";
  case kStopped:
    return "stopped";
  case kUnknown:
  default:
    return "unknown";
  }
}

void disposeProcess(Process * & p) {
  assert(NULL != p);
  //p->kill();
  delete p;
  p = NULL;
}

struct ExecutionDetails {
  AudioDevice::Device audioDevice;
  Media::Type mediaType;
  int volume;

  ExecutionDetails(void) :
    audioDevice(AudioDevice::kUndefined),
    mediaType(Media::kUndefined),
    volume(0) { }
};

void execute(Process * & p, const char * const v, const ExecutionDetails * const d) {
  assert(NULL != v);
  assert(NULL != d);

  if (NULL != p) {
    disposeProcess(p);
  }

  assert(NULL == p);

  p = new Process();

  static const char * const PLAYER = "/usr/bin/omxplayer";

  typedef std::vector< const char * > Arguments;
  Arguments arguments;
  arguments.push_back(PLAYER);

  if (d->mediaType != Media::kAudio) {
    arguments.push_back("--blank");
  } else {
    arguments.push_back("--no-osd");
  }

  arguments.push_back("--with-info");

  if (AudioDevice::kUndefined != d->audioDevice) {
    d->audioDevice = AudioDevice::kBoth;
  }
  arguments.push_back("--adev");
  arguments.push_back(AudioDevice::print(d->audioDevice));

  arguments.push_back("--refresh");

  arguments.push_back("--font");
  arguments.push_back("/usr/share/fonts/TTF/DejaVuSans.ttf");

  arguments.push_back("--italic-font");
  arguments.push_back("/usr/share/fonts/TTF/DejaVuSans-Oblique.ttf");

  arguments.push_back("--vol");
  char volume[64] = { 0 };
  sprintf(volume, "%d", d->volume * 300);
  arguments.push_back(volume);

  arguments.push_back("--audio_queue");
  arguments.push_back("5");

  arguments.push_back("--video_queue");
  arguments.push_back("5");

  arguments.push_back("--threshold");
  arguments.push_back("30");

  arguments.push_back("--timeout");
  arguments.push_back("30");

  arguments.push_back(v);

  arguments.push_back(NULL);

  assert(!arguments.empty());
  p->execute(PLAYER, arguments.data());
}

Player::~Player(void) {
  if (NULL != process_) {
    disposeProcess(process_);
  }

  if (NULL != media_) {
    delete media_;
  }
}

Player::Player(void) : state_(kEnded), process_(NULL),
  media_(NULL), volume_(0), audioDevice_(AudioDevice::kUndefined) { }

void Player::play(const Media & m) {
  const State s = state();
  if (kPlaying == s || kPaused == s) {
    assert(NULL != process_);
    stop();
  }

  //addressing repeat
  if (&m != media_) {
    assert(NULL != m.location());
    if (NULL != media_) {
      delete media_;
    }
    media_ = new Media(m);
  }

  ExecutionDetails d;

  d.audioDevice = audioDevice_;
  d.mediaType = media_->type();
  d.volume = volume_;

  execute(process_, media_->location(), &d);

  if ( ! process_->exists()) {
    disposeProcess(process_);
  } else {
    state_ = kPlaying;
  }
}

void Player::end(void) {
  if (NULL != process_) {
    process_->write("q");
    disposeProcess(process_);
  }
  state_ = kEnded;
}

void Player::stop(void) {
  const State s = state();
  if (NULL == process_ || kStopped == s || kEnded == s) {
    return;
  }
  process_->write("q");
  state_ = kStopped;
  disposeProcess(process_);
}

void Player::pause(void) {
  if (NULL == process_) { return; }
  if (kPlaying != state()) { return; }
  process_->write("p"); //pause
  state_ = kPaused;
}

void Player::resume(void) {
  if (NULL == process_) { return; }
  if (kPaused != state_) { return; }
  process_->write("p"); //pause
  state_ = kPlaying;
}

Player::State Player::state(void) {
  if (NULL != process_) {
    switch (state_) {
    case kPaused:
      if ( ! process_->exists()) {
        state_ = kCrashed;
      }
      break;

    case kPlaying:
      if ( ! process_->exists()) {
        state_ = kEnded;
        assert(NULL != media_);
        const int repeat = media_->repeating();
        if (0 < repeat) {
          std::cout << "repeating " << repeat << std::endl;
          play(*media_);
        }
      }
      break;

    default:
      break;
    }
  }
  return state_;
}

Media * Player::media(void) const {
  return media_;
}

void Player::volumeUp(void) {
  ++volume_;
  if (NULL == process_) { return; }
  process_->write("+"); //volume up
}

void Player::volumeDown(void) {
  --volume_;
  if (NULL == process_) { return; }
  process_->write("-"); //volume down
}

void Player::forward(const int s) const {
  if (NULL == process_) { return; }
  for (int i = s / 600; i > 0; --i) {
    process_->write("\x1b\x5b\x41"); //forward 600 seconds
    usleep(50000);
  }

  for (int i = (s % 600) / 30 ; i > 0; --i) {
    process_->write("\x1b\x5b\x43"); //forward 30 seconds
    usleep(50000);
  }
}

void Player::rewind(const int s) const {
  if (NULL == process_) { return; }
  for (int i = s / 600; i > 0; --i) {
    process_->write("\x1b\x5b\x42"); //rewind 600 seconds
    usleep(50000);
  }

  for (int i = (s % 600) / 30 ; i > 0; --i) {
    process_->write("\x1b\x5b\x44"); //rewind 30 seconds
    usleep(50000);
  }
}

void Player::chapterNext(void) const {
  if (NULL == process_) { return; }
  process_->write("o"); //next chapter
}

void Player::chapterPrevious(void) const {
  if (NULL == process_) { return; }
  process_->write("i"); //previous chapter
}

void Player::showInfo(void) const {
  if (NULL == process_) { return; }
  process_->write("z"); //show info
}

void Player::nextSubtitleStream(void) const {
  if (NULL == process_) { return; }
  process_->write("m"); //next subtitle stream
}

void Player::speedIncrease(void) const {
  if (NULL == process_) { return; }
  process_->write("2"); //increase speed
}

void Player::speedDecrease(void) const {
  if (NULL == process_) { return; }
  process_->write("1"); //decrease speed
}

} //end of oas namespace
