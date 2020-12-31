#include "StringFormatter.h"

namespace GameEngine {
  template <typename T>
  ArgumentType<T>::ArgumentType(T const& t): mData(t) {
  }

  template <typename T>
  void ArgumentType<T>::append(std::string& dest) const {
    dest.append(StringUtils::normalize(mData));
  }
}