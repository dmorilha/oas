#include "trivalue.h"

const char * print(const TriValue t) {
  switch (t) {
  case kFalse:
    return "false";
  case kTrue:
    return "true";
  case kUndefined:
  default:
    return "undefined";
  }
}
