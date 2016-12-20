#ifndef HS200_H
#define HS200_H

#include "lights.h"
#include <string>

namespace oas {
struct HS200 : Lights {
  const int port_;
  const std::string address_;

  HS200(const char * const, const int p = 9999);

  void on(void);
  void off(void);
};
} //end of oas namespace
#endif //HS200_H
