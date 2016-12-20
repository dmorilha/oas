#include "lights.h"

namespace oas {
const char * Lights::print(const Lights::State s) {
  switch (s) {
  case kOn:
    return "on";
  case kOff:
    return "off";
  case kUnknown:
  default:
    return "unknown";
  }
}

Lights::Lights(void) : state_(kUnknown) { }

Lights::State Lights::state(void) const {
  return state_;
}

} //end of oas namespace
