#ifndef GLADOS_VEC_H
#define GLADOS_VEC_H

#include <cstddef>

namespace GLaDOS {
    template <typename T, std::size_t N>
    class Vec {
      public:


        T v[N];
    };
}

#endif  // GLADOS_VEC_H
