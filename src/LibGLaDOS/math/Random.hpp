#ifndef GLADOS_RANDOM_HPP
#define GLADOS_RANDOM_HPP

#include <random>

#include "utils/Enumeration.h"

namespace GLaDOS {
    class Random {
      public:
        Random() = delete;
        ~Random() = delete;

        static bool nextBool();
        template <typename T>
        static T nextReal(T to);
        template <typename T>
        static T nextReal(T from, T to);
        template <typename T>
        static T nextInt(T to);
        template <typename T>
        static T nextInt(T from, T to);

        static bool below(real percent);
        static bool above(real percent);
    };

    template <typename T>
    T Random::nextReal(T to) {
        return nextReal<T>(0, to);
    }

    template <typename T>
    T Random::nextReal(T from, T to) {
        if (from >= to) {
            return from;
        }

        std::uniform_real_distribution<T> range(from, to);
        std::random_device randomDevice;
        std::default_random_engine engine{randomDevice()};

        return range(engine);
    }

    template <typename T>
    T Random::nextInt(T to) {
        return nextInt<T>(0, to);
    }

    template <typename T>
    T Random::nextInt(T from, T to) {
        if (from >= to) {
            return from;
        }

        std::uniform_int_distribution<T> range(from, to - 1);
        std::random_device randomDevice;
        std::default_random_engine engine{randomDevice()};

        return range(engine);
    }
}  // namespace GLaDOS

#endif  // GLADOS_RANDOM_HPP
