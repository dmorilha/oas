#include <assert.h>
#include <iostream>
#include <list>

#include "tv.h"

namespace oas {

struct Adapter {
  CEC::ICECAdapter * adapter_;

  Adapter(void) : adapter_(NULL) { }

  ~Adapter() {
    if (NULL != adapter_) {
      close();
    }
  }

  bool ping(void) const {
    return NULL != adapter_ && adapter_->IsActiveDeviceType(CEC::CEC_DEVICE_TYPE_TV);
  };

  void open(void) {
    using namespace CEC;

    libcec_configuration * const config = new libcec_configuration();
    config->Clear();

    strcpy(config->strDeviceName, "OAS");
    config->clientVersion = LIBCEC_VERSION_CURRENT;
    config->bActivateSource = 0;

    config->deviceTypes.Add(CEC_DEVICE_TYPE_PLAYBACK_DEVICE);

    config->callbacks = new ICECCallbacks();
    config->callbacks->Clear();

    adapter_ = CECInitialise(config);
    assert(NULL != adapter_);

    adapter_->InitVideoStandalone();

    cec_adapter_descriptor * const adapterList = static_cast< cec_adapter_descriptor * >(
        malloc(sizeof(cec_adapter_descriptor)));

    bool result = false;

    if (adapter_->DetectAdapters(adapterList, 1) > 0) {
      result = adapter_->Open(adapterList[0].strComName, 0);
    }

    assert(result);
    free(adapterList);
  }

  void close(void) {
    assert(NULL != adapter_);
    CECDestroy(adapter_);
  }

  CEC::ICECAdapter * get(void) {
    if ( ! ping()) {
      open();
    }
    return adapter_;
  }
};

const char * TV::print(const TV::State s) {
  switch (s) {
  case kOn:
    return "on";
  case kStandby:
    return "stand-by";
  case kUnknown:
  default:
    return "unknown";
  }
}

TV::~TV(void) {
  assert(NULL != adapter_);
  delete adapter_;
}

TV::TV(void) :
  state_(kUnknown),
  adapter_(new Adapter),
  logicalAddress_(CEC::CECDEVICE_TV) {
  using namespace CEC;


  state_ = getDevicePowerStatus();
}

TV::State TV::state(void) const {
  return state_;
}

TriValue TV::on(void) {
  assert(NULL != adapter_);
  CEC::ICECAdapter * const adapter = adapter_->get();
  if (NULL == adapter) { return kUndefined; }
  const bool result = adapter->PowerOnDevices();
  if (result) { state_ = kOn; }
  return result ? kTrue : kFalse;
}

TriValue TV::standby(void) {
  assert(NULL != adapter_);
  CEC::ICECAdapter * const adapter = adapter_->get();
  if (NULL == adapter) { return kUndefined; }
  const bool result = adapter->StandbyDevices();
  if (result) { state_ = kStandby; }
  return result ? kTrue : kFalse;
}

TriValue TV::setActiveSource(void) const {
  assert(NULL != adapter_);
  CEC::ICECAdapter * const adapter = adapter_->get();
  if (NULL == adapter) { return kUndefined; }
  return adapter->SetActiveSource() ? kTrue : kFalse;
}

TV::State TV::getDevicePowerStatus(void) const {
  using namespace CEC;

  assert(NULL != adapter_);
  ICECAdapter * const adapter = adapter_->get();
  if (NULL == adapter) { return kUnknown; }

  switch (adapter->GetDevicePowerStatus(logicalAddress_)) {
  case CEC_POWER_STATUS_ON:
  case CEC_POWER_STATUS_IN_TRANSITION_STANDBY_TO_ON:
    return kOn;

  case CEC_POWER_STATUS_STANDBY:
  case CEC_POWER_STATUS_IN_TRANSITION_ON_TO_STANDBY:
    return kStandby;

  case CEC_POWER_STATUS_UNKNOWN:
  default:
    return kUnknown;
  }
}

TriValue TV::pingAdapter(void) const {
  assert(NULL != adapter_);
  CEC::ICECAdapter * const adapter = adapter_->get();
  if (NULL == adapter) { return kUndefined; }
  return adapter->IsActiveDeviceType(CEC::CEC_DEVICE_TYPE_TV) ? kTrue : kFalse;
}

TriValue TV::poolDevice(void) const {
  assert(NULL != adapter_);
  CEC::ICECAdapter * const adapter = adapter_->get();
  if (NULL != adapter) { return kUndefined; }
  return adapter->PollDevice(logicalAddress_) ? kTrue : kFalse;
}

TriValue TV::isActiveDevice(void) const {
  assert(NULL != adapter_);
  CEC::ICECAdapter * const adapter = adapter_->get();
  if (NULL != adapter) { return kUndefined; }
  return adapter->IsActiveDevice(logicalAddress_) ? kTrue : kFalse;
}

void TV::volumeDown(void) const {
  assert(NULL != adapter_);
  CEC::ICECAdapter * const adapter = adapter_->get();
  if (NULL != adapter) { return; }
  adapter->VolumeDown();
}

void TV::volumeUp(void) const {
  assert(NULL != adapter_);
  CEC::ICECAdapter * const adapter = adapter_->get();
  if (NULL != adapter) { return; }
  adapter->VolumeUp();
}

TriValue TV::isLibCECActiveSource(void) const {
  assert(NULL != adapter_);
  CEC::ICECAdapter * const adapter = adapter_->get();
  if (NULL != adapter) { return kUndefined; }
  return adapter->IsLibCECActiveSource() ? kTrue : kFalse;
}

} // end of oas namespace
