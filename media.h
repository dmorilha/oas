#ifndef MEDIA_H
#define MEDIA_H

#include <string>

namespace oas {

struct Media {
  enum Type {
    kUndefined,
    kAudio,
    kVideo,
  };

  static const char * print(const Type);
  static Type fromString(const char *);

  std::string location_;
  Type type_;

  Media(void);
  Media(const char * const, const Type t = kUndefined);

  void swap(Media &);

  const char * location(void) const;
  Type type(void) const;
};

} //end of oas namespace
#endif //MEDIA_H
