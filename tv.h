#ifndef tv_h
#define tv_h

#include <libcec/cec.h>

namespace oas {

struct TV {
  enum State {
    kUndefined,
    kOn,
    kStandby,
  };

  State state_;

  CEC::ICECAdapter * adapter_;

  ~TV();
  TV(void);

  State state(void) const;

  bool on(void);
  bool setActiveSource(void) const;
  bool standby(void);
};

} //end of oas namespace

#endif //tv_h
