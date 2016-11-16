#include <assert.h>
#include <iostream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dbus.h"

/*
void reply_to_method_call(DBusMessage* msg, DBusConnection* conn) {
   DBusMessage* reply;
   DBusMessageIter args;
   bool stat = true;
   dbus_uint32_t level = 21614;
   dbus_uint32_t serial = 0;
   char* param = "";

   // read the arguments
   if (!dbus_message_iter_init(msg, &args))
      fprintf(stderr, "Message has no arguments!\n");
   else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
      fprintf(stderr, "Argument is not string!\n");
   else
      dbus_message_iter_get_basic(&args, &param);

   printf("Method called with %s\n", param);

   // create a reply from the message
   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &stat)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &level)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }

   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   dbus_connection_flush(conn);

   // free the reply
   dbus_message_unref(reply);
}
*/

namespace oas {

bool DBUS::checkErrors(const char * const p) {
  if (dbus_error_is_set(&error_)) {

    if (NULL != p) { std::cerr << p << " "; }

    std::cerr << "(" << error_.message << ")" << std::endl;
    dbus_error_free(&error_);

    return true;
  }

  return false;
}

DBUS::~DBUS(void) {
  /*
  if (NULL != connection_) {
    dbus_connection_close(connection_);
  }
  */
}

DBUS::DBUS(void) {
   dbus_error_init(&error_);
   connection_ = dbus_bus_get(DBUS_BUS_SYSTEM, &error_);
   checkErrors("Connection Error");
   if (NULL == connection_) {
     std::cerr << "Connection Error" << std::endl;
   }
}

void DBUS::listen(void) {
  if (NULL == connection_) {
    return;
  }

  // for each in the list
  const int result = dbus_bus_request_name(connection_, "oas.methods.server",
      DBUS_NAME_FLAG_REPLACE_EXISTING , &error_);

  checkErrors("Name Error");

  if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != result) {
    std::cerr << "Not Primary Owner (" << result << ")" << std::endl;
  }
}

void DBUS::processMessages(void) {
  if (NULL == connection_) { return; }

  while (true) {
    // non blocking read of the next available message
    dbus_connection_read_write(connection_, 0);
    DBusMessage * const message = dbus_connection_pop_message(connection_);

    if (NULL == message) {
      break;
    }

#define METHOD(M, S) \
  if (dbus_message_is_method_call(message, S, #M)) { \
    M(message); \
  } else

    DBUS_METHOD_LIST(METHOD)

#undef METHOD

    {
      std::cerr << "Method not found." << std::endl;
    }

    // free the message
    dbus_message_unref(message);
  }
}

void DBUS::clear(DBusMessage * const m) { }
void DBUS::next(DBusMessage * const m) { }

void DBUS::play(DBusMessage * const m) {
  DBusMessageIter arguments;
  const char * value = NULL;

  if ( ! dbus_message_iter_init(m, &arguments)) {
    std::cerr << "Method was called with no parameters" << std::endl;

  } else if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&arguments)) {
    dbus_message_iter_get_basic(&arguments, &value);
    //TODO(dmorilha): should be assert?
    if (NULL != value) {
      std::cout << "argument value is: " << value << std::endl;
    }

  } else {
    std::cerr << "Argument is not string" << std::endl;
  }
}

void DBUS::previous(DBusMessage * const m) { }
void DBUS::pushBack(DBusMessage * const m) { }
void DBUS::pushFront(DBusMessage * const m) { }

} //end of oas namespace
