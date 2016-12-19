#include <cstring>

#include "media.h"

namespace oas {

const char * Media::print(const Type t) {
  switch (t) {
  case kAudio:
    return "audio";
  case kVideo:
    return "video";
  case kUndefined:
  default:
    return "undefined";
  }
}

Media::Type Media::fromString(const char * const s) {
  if (NULL != s) {
    if (0 == strcmp(s, "audio")) {
      return Media::kAudio;
    } else if (0 == strcmp(s, "video")) {
      return Media::kVideo;
    }
  }

  return Media::kUndefined;
}

Media::Media(void) : type_(kUndefined), repeat_(0) { }

Media::Media(const char * const l, const Type t) :
  location_(l), type_(t), repeat_(0) { }

void Media::swap(Media & m) {
  location_.swap(m.location_);

  {
    const Type t = m.type_;
    m.type_ = type_;
    type_ = t;
  }

  {
    const int r = m.repeat_;
    m.repeat_ = repeat_;
    repeat_ = r;
  }
}

const char * Media::location(void) const {
  return location_.c_str();
}

Media::Type Media::type(void) const {
  return type_;
}

void Media::repeat(const int i) {
  repeat_ += i;
}

int Media::repeating(void) {
  return repeat_--;
}

} //end of oas namespace
