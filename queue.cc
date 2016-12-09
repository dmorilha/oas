#include "queue.h"

namespace oas {

bool Queue::empty(void) const {
  return priorityQueue_.empty() && queue_.empty();
}

bool Queue::next(std::string & s) {
  if ( ! priorityQueue_.empty()) {
    s.swap(priorityQueue_.back());
    priorityQueue_.pop_back();
  } else if ( ! queue_.empty()) {
    s.swap(queue_.back());
    queue_.pop_back();
  } else {
    return false;
  }
  return true;
}

void Queue::pushBack(const char * const s) {
  queue_.push_back(s);
}

void Queue::pushFront(const char * const s) {
  priorityQueue_.push_back(s);
}

} //end of aos namepsace
