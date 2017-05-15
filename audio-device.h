#ifndef AUDIO_DEVICE_H
#define AUDIO_DEVICE_H

namespace oas {
struct AudioDevice {
  enum Device {
    kUndefined,
    kAlsa,
    kBoth,
    kHDMI,
    kLocal,
  };

  static const char * print(const Device);
  static Device fromString(const char * const);
};
} //end of oas namespace
#endif //AUDIO_DEVICE_H
