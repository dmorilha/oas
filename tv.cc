#include <libcec/cec.h>

#include "tv.h"

namespace oas {

/*
static ICECAdapter adapter_;
extern "C" DECLSPEC void CECDestroy(CEC::ICECAdapter *instance);
extern "C" DECLSPEC CEC::ICECAdapter* CECInitialise(CEC::libcec_configuration *configuration);
 */

void TV::on(void) {
    //virtual bool PowerOnDevices(cec_logical_address address = CECDEVICE_TV) = 0;
}

void TV::off(void) {
    //virtual bool StandbyDevices(cec_logical_address address = CECDEVICE_BROADCAST) = 0;
}

} // end of oas namespace
