#ifndef tv_h
#define tv_h

#include <libcec/cec.h>

#include "trivalue.h"

namespace oas {

struct TV {
  enum State {
    kUnknown,
    kOn,
    kStandby,
  };

  static const char * print(State);

  State state_;

  CEC::ICECAdapter * adapter_;

  const CEC::cec_logical_address logicalAddress_;

  ~TV();
  TV(void);

  State state(void) const;

  TriValue on(void);
  TriValue setActiveSource(void) const;
  TriValue standby(void);
  State getDevicePowerStatus(void) const;
  TriValue pingAdapter(void) const;
  TriValue poolDevice(void) const;
  TriValue isActiveDevice(void) const;
  void volumeDown(void) const;
  void volumeUp(void) const;
  TriValue isLibCECActiveSource(void) const;
};

} //end of oas namespace

#endif //tv_h
