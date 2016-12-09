#include <assert.h>
#include <iostream>
#include <list>

#include "tv.h"

namespace oas {

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
  adapter_->Close();
  CECDestroy(adapter_);
}

TV::TV(void) : state_(kUnknown), adapter_(NULL),
  logicalAddress_(CEC::CECDEVICE_TV) {
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

  if ( ! result) {
    adapter_ = NULL;
  }

  free(adapterList);

  state_ = getDevicePowerStatus();
}

TV::State TV::state(void) const {
  if (NULL == adapter_) { return kUnknown; }
  return state_;
}

TriValue TV::on(void) {
  if (NULL == adapter_) { return kUndefined; }
  const bool result = adapter_->PowerOnDevices(CEC::CECDEVICE_BROADCAST);
  if (result) {
    state_ = kOn;
  }
  return result ? kTrue : kFalse;
}

TriValue TV::standby(void) {
  if (NULL == adapter_) { return kFalse; }
  const bool result = adapter_->StandbyDevices();
  if (result) {
    state_ = kStandby;
  }
  return result ? kTrue : kFalse;
}

TriValue TV::setActiveSource(void) const {
  if (NULL == adapter_) { return kUndefined; }
  return adapter_->SetActiveSource() ? kTrue : kFalse;
}

TV::State TV::getDevicePowerStatus(void) const {
  using namespace CEC;

  if (NULL == adapter_) { return kUnknown; }
  switch (adapter_->GetDevicePowerStatus(logicalAddress_)) {
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
  if (NULL == adapter_) { return kUndefined; }
  return adapter_->PingAdapter() ? kTrue : kFalse;
}

TriValue TV::poolDevice(void) const {
  if (NULL != adapter_) { return kUndefined; }
  return adapter_->PollDevice(logicalAddress_) ? kTrue : kFalse;
}

TriValue TV::isActiveDevice(void) const {
  if (NULL != adapter_) { return kUndefined; }
  return adapter_->IsActiveDevice(logicalAddress_) ? kTrue : kFalse;
}

void TV::volumeDown(void) const {
  if (NULL != adapter_) { return; }
  adapter_->VolumeDown();
}

void TV::volumeUp(void) const {
  if (NULL != adapter_) { return; }
  adapter_->VolumeUp();
}

TriValue TV::isLibCECActiveSource(void) const {
  if (NULL != adapter_) { return kUndefined; }
  return adapter_->IsLibCECActiveSource() ? kTrue : kFalse;
}

} // end of oas namespace
