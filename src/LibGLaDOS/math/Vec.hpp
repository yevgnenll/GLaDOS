#ifndef GLADOS_VEC_H
#define GLADOS_VEC_H

#include <cstddef>

namespace GLaDOS {
    template <typename T, std::size_t D>
    class Vec {
      public:


        T v[D];
    };
}

#endif  // GLADOS_VEC_H
