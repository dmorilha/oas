#include "queue.h"

namespace oas {

void Queue::push(const char * const s) {
  queue_.push_back(s);
}

void Queue::pushFront(const char * const s) {
  priorityQueue_.push_back(s);
}

bool Queue::next(std::string & s) {
  if ( ! priorityQueue_.empty()) {
    s.swap(queue_.back());
    queue_.pop_back();
  } else if ( ! queue_.empty()) {
    s.swap(queue_.back());
    queue_.pop_back();
  } else {
    return false;
  }
  return true;
}

} //end of aos namepsace
