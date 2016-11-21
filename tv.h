#ifndef tv_h
#define tv_h

#include <libcec/cec.h>

namespace oas {

struct TV {
  CEC::ICECAdapter * adapter_;

  ~TV();
  TV(void);
  void on(void) const;
  void standby(void) const;
};

} //end of oas namespace

#endif //tv_h
