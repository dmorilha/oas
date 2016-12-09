#ifndef queue_h
#define queue_h

#include <string>
#include <vector>

namespace oas {

struct Queue {
  typedef std::vector< std::string > Container;

  Container queue_, priorityQueue_;

  bool empty(void) const;
  bool next(std::string &);
  void pushBack(const char * const);
  void pushFront(const char * const);
};

} //end of aos namepsace

#endif //queue_h
