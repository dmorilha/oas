#ifndef queue_h
#define queue_h

#include <string>
#include <vector>

namespace oas {

struct Queue {
  typedef std::vector< std::string > Container;

  Container queue_, priorityQueue_;

  void push(const char * const);
  void pushFront(const char * const);

  bool next(std::string &);
};

} //end of aos namepsace

#endif //queue_h
