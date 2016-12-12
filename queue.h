#ifndef queue_h
#define queue_h

#include <string>
#include <vector>

#include "media.h"

namespace oas {

struct Queue {
  typedef std::vector< Media > Container;

  Container queue_, priorityQueue_;

  bool empty(void) const;
  bool next(Media &);
  void pushBack(const Media &);
  void pushFront(const Media &);
  size_t size(void) const;
};

} //end of aos namepsace

#endif //queue_h
