#ifndef TRIVALUE_H
#define TRIVALUE_H
enum TriValue {
  kFalse,
  kTrue,
  kUndefined = -1,
};

const char * print(const TriValue);
#endif //TRIVALUE_H
