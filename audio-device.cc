#include <cstring>

#include "audio-device.h"

namespace oas {

const char * AudioDevice::print(const AudioDevice::Device d) {
  switch (d) {
  case kAlsa:
    return "alsa";
  case kBoth:
    return "both";
  case kHDMI:
    return "hdmi";
  case kLocal:
    return "local";
  case kUndefined:
  default:
    return "undefined";
  }
}

AudioDevice::Device AudioDevice::fromString(const char * const a) {
  if (NULL != a) {
    if (0 == strcmp(a, "alsa")) {
      return kAlsa;
    } else if (0 == strcmp(a, "both")) {
      return kBoth;
    } else if (0 == strcmp(a, "hdmi")) {
      return kHDMI;
    } else if (0 == strcmp(a, "local")) {
      return kLocal;
    }
  }
  return kUndefined;
}

} //end of oas namespace
