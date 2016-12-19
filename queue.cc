#include "queue.h"

namespace oas {

bool Queue::empty(void) const {
  return priorityQueue_.empty() && queue_.empty();
}

bool Queue::next(Media & m) {
  if ( ! priorityQueue_.empty()) {
    m.swap(priorityQueue_.back());
    priorityQueue_.pop_back();
  } else if ( ! queue_.empty()) {
    m.swap(queue_.front());
    queue_.pop_front();
  } else {
    return false;
  }
  return true;
}

void Queue::pushBack(const Media & m) {
  queue_.push_back(m);
}

void Queue::pushFront(const Media & m) {
  priorityQueue_.push_back(m);
}

size_t Queue::size(void) const {
  return priorityQueue_.size() + queue_.size();
}

void Queue::clear(void) {
  priorityQueue_.clear();
  queue_.clear();
}

} //end of aos namepsace
