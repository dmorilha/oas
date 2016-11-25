#ifndef MY_DBUS_H
#define MY_DBUS_H

#define DBUS_API_SUBJECT_TO_CHANGE
#include <dbus/dbus.h>

#include "player.h"

namespace oas {

static const char * const DBUS_METHOD_INTEGER = "oas.method.integer";
static const char * const DBUS_METHOD_STRING = "oas.method.string";
static const char * const DBUS_METHOD_VOID = "oas.method.void";

#define DBUS_METHOD_LIST(V) \
  V(clear, DBUS_METHOD_VOID) \
  V(forward30, DBUS_METHOD_VOID) \
  V(forward600, DBUS_METHOD_VOID) \
  V(next, DBUS_METHOD_VOID) \
  V(pause, DBUS_METHOD_VOID) \
  V(play, DBUS_METHOD_STRING) \
  V(previous, DBUS_METHOD_VOID) \
  V(pushBack, DBUS_METHOD_STRING) \
  V(pushFront, DBUS_METHOD_STRING) \
  V(resume, DBUS_METHOD_VOID) \
  V(rewind30, DBUS_METHOD_VOID) \
  V(rewind600, DBUS_METHOD_VOID) \
  V(stop, DBUS_METHOD_VOID) \
  V(volumeDown, DBUS_METHOD_VOID) \
  V(volumeUp, DBUS_METHOD_VOID) \

struct DBUS {
  DBusError error_;
  DBusConnection * connection_;
  Player * player_;

  ~DBUS(void);
  DBUS(Player * const);

  void listen(void);
  void processMessages(void);

  bool checkErrors(const char * const prefix = NULL);

#define DBUS_METHOD_DECLARATION(M, S) \
  void M(DBusMessage * const); \

DBUS_METHOD_LIST(DBUS_METHOD_DECLARATION)

};
} //end of oas namespace

#endif //MY_DBUS_H
