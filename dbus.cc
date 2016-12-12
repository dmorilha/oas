#include <assert.h>
#include <iostream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dbus.h"
#include "player.h"
#include "queue.h"
#include "tv.h"

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
  //TODO(dmorilha): causes an error
  /*
  if (NULL != connection_) {
    dbus_connection_close(connection_);
  }
  */
}

DBUS::DBUS(Player * const p, Queue * const q, TV * const t) :
  player_(p), queue_(q), tv_(t) {
  assert(NULL != player_);
  assert(NULL != queue_);
  assert(NULL != tv_);
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

    /*
     * yah I know this looks strange...
     */
    {
      std::cerr << "Method not found." << std::endl;
    }

    // free the message
    dbus_message_unref(message);
  }
}

void DBUS::clear(DBusMessage * const m) { }

void DBUS::next(DBusMessage * const m) { }

bool parseMediaArguments(DBusMessage * const m, Media & me) {
  DBusMessageIter arguments;

  if ( ! dbus_message_iter_init(m, &arguments)) {
    std::cerr << "Method was called with no parameters" << std::endl;

  } else if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&arguments)) {
    const char * location = NULL;
    dbus_message_iter_get_basic(&arguments, &location);

    if (NULL != location) {
      const char * type = NULL;
      if (dbus_message_iter_next(&arguments)) {
        if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&arguments)) {
          dbus_message_iter_get_basic(&arguments, &type);
          //TODO(dmorilha): should be assert?
        }
      }

      me = Media(location, Media::fromString(type));
      return true;
    }

  } else {
    std::cerr << "First argument is not string" << std::endl;
  }
  return false;
}

void DBUS::play(DBusMessage * const m) {
  Media media;
  if (parseMediaArguments(m, media)) {
    assert(NULL != player_);
    player_->play(media);
  }
}

void DBUS::previous(DBusMessage * const m) { }

void DBUS::pushBack(DBusMessage * const m) {
  Media media;
  if (parseMediaArguments(m, media)) {
    assert(NULL != queue_);
    queue_->pushBack(media);
  }
}

void DBUS::pushFront(DBusMessage * const m) {
  Media media;
  if (parseMediaArguments(m, media)) {
    assert(NULL != queue_);
    queue_->pushFront(media);
  }
}

void DBUS::stop(DBusMessage * const m) {
  std::cout << "stop" << std::endl;
  assert(NULL != player_);
  player_->stop();
}

void DBUS::volumeDown(DBusMessage * const m) {
  std::cout << "volume down" << std::endl;
  assert(NULL != player_);
  player_->volumeDown();
}

void DBUS::volumeUp(DBusMessage * const m) {
  std::cout << "volume up" << std::endl;
  assert(NULL != player_);
  player_->volumeUp();
}

void DBUS::forward30(DBusMessage * const m) {
  std::cout << "forward 30 seconds" << std::endl;
  assert(NULL != player_);
  player_->forward30();
}

void DBUS::forward600(DBusMessage * const m) {
  std::cout << "forward 600 seconds" << std::endl;
  assert(NULL != player_);
  player_->forward600();
}

void DBUS::resume(DBusMessage * const m) {
  std::cout << "resume" << std::endl;
  assert(NULL != player_);
  player_->resume();
}

void DBUS::rewind30(DBusMessage * const m) {
  std::cout << "rewind 30 seconds" << std::endl;
  assert(NULL != player_);
  player_->rewind30();
}

void DBUS::rewind600(DBusMessage * const m) {
  std::cout << "rewind 600 seconds" << std::endl;
  assert(NULL != player_);
  player_->rewind600();
}

void DBUS::pause(DBusMessage * const m) {
  std::cout << "pause" << std::endl;
  assert(NULL != player_);
  player_->pause();
}

void DBUS::turnOn(DBusMessage * const m) {
  std::cout << "turn on" << std::endl;
  assert(NULL != tv_);
  tv_->on();
}

void DBUS::turnOff(DBusMessage * const m) {
  std::cout << "turn off" << std::endl;
  assert(NULL != tv_);
  tv_->standby();
}

} //end of oas namespace
