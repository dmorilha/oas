#include <assert.h>

#include "tv.h"

namespace oas {

TV::~TV(void) {
  assert(NULL != adapter_);
  CECDestroy(adapter_);
}

TV::TV(void) : adapter_(NULL) {
  CEC::libcec_configuration configuration;
  adapter_ = CECInitialise(&configuration);
  assert(NULL != adapter_);
}

void TV::on(void) const {
  assert(NULL != adapter_);
  //virtual bool PowerOnDevices(cec_logical_address address = CECDEVICE_TV) = 0;
  const bool result = adapter_->PowerOnDevices(CEC::CECDEVICE_BROADCAST);
}

void TV::standby(void) const {
  assert(NULL != adapter_);
  //virtual bool StandbyDevices(cec_logical_address address = CECDEVICE_BROADCAST) = 0;
  const bool result = adapter_->StandbyDevices();
}

} // end of oas namespace
