#ifndef queue_h
#define queue_h

#include <deque>
#include <string>
#include <vector>

#include "media.h"

namespace oas {

struct Queue {

  std::deque< Media > queue_;
  std::vector< Media > priorityQueue_;

  bool empty(void) const;
  bool next(Media &);
  void pushBack(const Media &);
  void pushFront(const Media &);
  size_t size(void) const;
  void clear(void);
};

} //end of aos namepsace

#endif //queue_h
