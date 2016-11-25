#include <assert.h>
#include <iostream>
#include <list>

#include "tv.h"

namespace oas {

TV::~TV(void) {
  assert(NULL != adapter_);
  adapter_->Close();
  CECDestroy(adapter_);
}

TV::TV(void) : state_(kUndefined), adapter_(NULL) {
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
    result = adapter_->Open(adapterList[0].strComName);
  }

  if ( ! result) {
    adapter_ = NULL;
  }

  free(adapterList);
}

TV::State TV::state(void) const {
  if (NULL == adapter_) { return kUndefined; }
  return state_;
}

bool TV::on(void) {
  if (NULL == adapter_) { return false; }
  const bool result = adapter_->PowerOnDevices(CEC::CECDEVICE_BROADCAST);
  if (result) {
    state_ = kOn;
  }
  return result;
}

bool TV::standby(void) {
  if (NULL == adapter_) { return false; }
  const bool result = adapter_->StandbyDevices();
  if (result) {
    state_ = kStandby;
  }
  return result;
}

bool TV::setActiveSource(void) const {
  if (NULL == adapter_) { return false; }
  return adapter_->SetActiveSource();
}

} // end of oas namespace
