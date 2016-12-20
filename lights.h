#ifndef LIGHTS_H
#define LIGHTS_H

namespace oas {
struct Lights {
  enum State {
    kUnknown,
    kOn,
    kOff,
  };

  State state_;
  static const char * print(const State s);

  Lights(void);

  State state(void) const;
  virtual void on(void) = 0;
  virtual void off(void) = 0;
};

} //end of oas namespace
#endif //LIGHTS_H
