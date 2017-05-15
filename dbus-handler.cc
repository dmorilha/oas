#include <assert.h>
#include <iostream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "controller.h"
#include "dbus-handler.h"
#include "lights.h"
#include "player.h"
#include "queue.h"
#include "tv.h"

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

DBUS::DBUS(Controller * const c) :
  controller_(c) {
  assert(NULL != controller_);
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
    assert(NULL != controller_);
    controller_->play(media);
  }
}

void DBUS::pushBack(DBusMessage * const m) {
  Media media;
  if (parseMediaArguments(m, media)) {
    assert(NULL != controller_);
    controller_->pushBack(media);
  }
}

void DBUS::pushFront(DBusMessage * const m) {
  Media media;
  if (parseMediaArguments(m, media)) {
    assert(NULL != controller_);
    controller_->pushFront(media);
  }
}

void DBUS::setAudioDevice(DBusMessage * const m) {
  DBusMessageIter arguments;

  if ( ! dbus_message_iter_init(m, &arguments)) {
    std::cerr << "Method was called with no parameters" << std::endl;

  } else if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&arguments)) {
    const char * audioDevice = NULL;
    dbus_message_iter_get_basic(&arguments, &audioDevice);
    assert(NULL != controller_);
    controller_->setAudioDevice(AudioDevice::fromString(audioDevice));
  } else {
    std::cerr << "First argument is not string" << std::endl;
  }
}

//TODO(dmorilha): reduce repetition
void DBUS::clear(DBusMessage * const m) {
  assert(NULL != controller_);
  controller_->clear();
}

void DBUS::next(DBusMessage * const m) {
  assert(NULL != controller_);
  controller_->next();
}

void DBUS::previous(DBusMessage * const m) {
  assert(NULL != controller_);
  controller_->previous();
}

void DBUS::stop(DBusMessage * const m) {
  assert(NULL != controller_);
  controller_->stop();
}

void DBUS::volumeDown(DBusMessage * const m) {
  assert(NULL != controller_);
  controller_->volumeDown();
}

void DBUS::volumeUp(DBusMessage * const m) {
  assert(NULL != controller_);
  controller_->volumeUp();
}

void DBUS::forward(DBusMessage * const m) {
  DBusMessageIter arguments;

  assert(NULL != controller_);
  if ( ! dbus_message_iter_init(m, &arguments)) {
    controller_->forward(60);

  } else if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&arguments)) {
    const char * buffer = NULL;
    dbus_message_iter_get_basic(&arguments, &buffer);
    if (NULL != buffer) {
      const int seconds = atoi(buffer);
      if (0 < seconds) {
        controller_->forward(seconds);
      }
    }
  } else {
    std::cerr << "First argument is not string" << std::endl;
  }
}

void DBUS::repeat(DBusMessage * const m) {
  assert(NULL != controller_);
  controller_->repeat();
}

void DBUS::resume(DBusMessage * const m) {
  assert(NULL != controller_);
  controller_->resume();
}

void DBUS::rewind(DBusMessage * const m) {
  DBusMessageIter arguments;

  assert(NULL != controller_);
  if ( ! dbus_message_iter_init(m, &arguments)) {
    controller_->rewind(60);

  } else if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&arguments)) {
    const char * buffer = NULL;
    dbus_message_iter_get_basic(&arguments, &buffer);
    if (NULL != buffer) {
      const int seconds = atoi(buffer);
      if (0 < seconds) {
        controller_->rewind(seconds);
      }
    }
  } else {
    std::cerr << "First argument is not string" << std::endl;
  }
}

void DBUS::pause(DBusMessage * const m) {
  assert(NULL != controller_);
  controller_->pause();
}

void DBUS::tvOn(DBusMessage * const m) {
  assert(NULL != controller_);
  controller_->tvOn();
}

void DBUS::tvOff(DBusMessage * const m) {
  assert(NULL != controller_);
  controller_->tvOff();
}

void DBUS::lightsOn(DBusMessage * const s) {
  assert(NULL != controller_);
  controller_->lightsOn();
}

void DBUS::lightsOff(DBusMessage * const s) {
  assert(NULL != controller_);
  controller_->lightsOff();
}

void DBUS::chapterNext(DBusMessage * const s) {
  assert(NULL != controller_);
  controller_->chapterNext();
}

void DBUS::chapterPrevious(DBusMessage * const s) {
  assert(NULL != controller_);
  controller_->chapterPrevious();
}

void DBUS::showInfo(DBusMessage * const s) {
  assert(NULL != controller_);
  controller_->showInfo();
}

void DBUS::nextSubtitleStream(DBusMessage * const s) {
  assert(NULL != controller_);
  controller_->nextSubtitleStream();
}

void DBUS::speedIncrease(DBusMessage * const s) {
  assert(NULL != controller_);
  controller_->speedIncrease();
}

void DBUS::speedDecrease(DBusMessage * const s) {
  assert(NULL != controller_);
  controller_->speedDecrease();
}

} //end of oas namespace
