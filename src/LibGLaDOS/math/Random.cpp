#include "Random.hpp"

namespace GLaDOS {
    bool Random::nextBool() {
        return nextInt(2) == 1;
    }

    real Random::nextReal(real to) {
        return nextReal(real(0), to);
    }

    real Random::nextReal(real from, real to) {
        return next(from, to);
    }

    int Random::nextInt(int to) {
        return nextInt(int(0), to);
    }

    int Random::nextInt(int from, int to) {
        return next(from, to);
    }

    bool Random::below(real percent) {
        return nextReal(1.0f) < percent;
    }

    bool Random::above(real percent) {
        return nextReal(1.0f) > percent;
    }
}