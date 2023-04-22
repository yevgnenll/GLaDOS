#ifndef GLADOS_RANDOM_HPP
#define GLADOS_RANDOM_HPP

#include <random>

#include "utils/Enumeration.h"
#include "utils/Utility.h"

namespace GLaDOS {
    class Random {
      public:
        DISALLOW_COPY_AND_ASSIGN(Random);

        static bool nextBool();
        static real nextReal(real to);
        static real nextReal(real from, real to);
        static int nextInt(int to);
        static int nextInt(int from, int to);
        static bool below(real percent);
        static bool above(real percent);

      private:
        static real internalNextReal(real from, real to);
        static int internalNextInt(int from, int to);
    };
}  // namespace GLaDOS

#endif  // GLADOS_RANDOM_HPP
