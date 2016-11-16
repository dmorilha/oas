#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include "dbus.h"
#include "queue.h"
#include "tv.h"

static const int TEN_MINUTES = 60 * 10;

oas::DBUS dbus;

void loop(void) {
  using namespace oas;
  Queue queue;
  TV tv;
  std::string string;
  tv.on();

  for (int i = TEN_MINUTES; i > 0; --i) {
    std::cout << i << std::endl;
    /*
    player.play(string.c_str());
    player.wait();
    */
    dbus.processMessages();
    sleep(1);
  }

  //sleep(TEN_MINUTES);
  tv.off();
}

int main(void) {
  std::cout << "omxplayer-dbus" << std::endl;
  /*
  FILE * const stream = popen("/usr/bin/omxplayer /home/dmorilha/051\\ -\\ The\\ Contest\\ -\\ [DVD].avi", "we");
  pclose(stream);
  loop();
  */
  dbus.listen();
  loop();
  return 0;
}
