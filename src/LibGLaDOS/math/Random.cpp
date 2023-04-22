#include "Random.hpp"

namespace GLaDOS {
    bool Random::nextBool() {
        return nextInt(2) == 1;
    }

    real Random::nextReal(real to) {
        return nextReal(real(0), to);
    }

    real Random::nextReal(real from, real to) {
        return internalNextReal(from, to);
    }

    int Random::nextInt(int to) {
        return nextInt(int(0), to);
    }

    int Random::nextInt(int from, int to) {
        return internalNextInt(from, to);
    }

    bool Random::below(real percent) {
        return nextReal(1.0f) < percent;
    }

    bool Random::above(real percent) {
        return nextReal(1.0f) > percent;
    }

    real Random::internalNextReal(real from, real to) {
        if (from >= to) {
            return from;
        }

        std::uniform_real_distribution<real> range(from, to);
        std::random_device randomDevice;
        std::mt19937 engine{randomDevice()};

        return range(engine);
    }

    int Random::internalNextInt(int from, int to) {
        if (from >= to) {
            return from;
        }

        std::uniform_int_distribution<int> range(from, to);
        std::random_device randomDevice;
        std::mt19937 engine{randomDevice()};

        return range(engine);
    }
}