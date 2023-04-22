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
        template <typename T>
        static std::enable_if_t<std::is_floating_point_v<T>, T> next(T from, T to);
        template <typename T>
        static std::enable_if_t<!std::is_floating_point_v<T>, T> next(T from, T to);
    };

    template <typename T>
    std::enable_if_t<std::is_floating_point_v<T>, T> Random::next(T from, T to) {
        if (from >= to) {
            return from;
        }

        std::uniform_real_distribution<T> range(from, to);
        std::random_device randomDevice;
        std::mt19937 engine{randomDevice()};

        return range(engine);
    }

    template <typename T>
    std::enable_if_t<!std::is_floating_point_v<T>, T> Random::next(T from, T to) {
        if (from >= to) {
            return from;
        }

        std::uniform_int_distribution<T> range(from, to);
        std::random_device randomDevice;
        std::mt19937 engine{randomDevice()};

        return range(engine);
    }
}  // namespace GLaDOS

#endif  // GLADOS_RANDOM_HPP
