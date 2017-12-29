#ifndef DBUS_HANDLER_H
#define DBUS_HANDLER_H

#define DBUS_API_SUBJECT_TO_CHANGE
#include <dbus/dbus.h>

namespace oas {
static const char * const DBUS_METHOD_VOID = "oas.method.void";

#define DBUS_METHOD_LIST(V) \
  V(clear, DBUS_METHOD_VOID) \
  V(forward, DBUS_METHOD_VOID) \
  V(lightsOff, DBUS_METHOD_VOID) \
  V(lightsOn, DBUS_METHOD_VOID) \
  V(next, DBUS_METHOD_VOID) \
  V(pause, DBUS_METHOD_VOID) \
  V(play, DBUS_METHOD_VOID) \
  V(pressKey, DBUS_METHOD_VOID) \
  V(previous, DBUS_METHOD_VOID) \
  V(pushBack, DBUS_METHOD_VOID) \
  V(pushFront, DBUS_METHOD_VOID) \
  V(repeat, DBUS_METHOD_VOID) \
  V(resume, DBUS_METHOD_VOID) \
  V(rewind, DBUS_METHOD_VOID) \
  V(setAudioDevice, DBUS_METHOD_VOID) \
  V(stop, DBUS_METHOD_VOID) \
  V(tvOff, DBUS_METHOD_VOID) \
  V(tvOn, DBUS_METHOD_VOID) \
  V(volumeDown, DBUS_METHOD_VOID) \
  V(volumeUp, DBUS_METHOD_VOID) \

struct Controller;

struct DBUS {
  DBusError error_;
  DBusConnection * connection_;
  Controller * const controller_;

  ~DBUS(void);
  DBUS(Controller * const);

  void listen(void);
  void processMessages(void);

  bool checkErrors(const char * const prefix = NULL);

#define DBUS_METHOD_DECLARATION(M, S) \
  void M(DBusMessage * const); \

DBUS_METHOD_LIST(DBUS_METHOD_DECLARATION)

};
} //end of oas namespace

#endif //DBUS_HANDLER_H
