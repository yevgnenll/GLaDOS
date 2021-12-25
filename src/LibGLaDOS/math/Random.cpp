#include "Random.hpp"

namespace GLaDOS {
    bool Random::nextBool() {
        return nextInt(2) == 1;
    }

    bool Random::below(real percent) {
        return nextReal(1.0f) < percent;
    }

    bool Random::above(real percent) {
        return nextReal(1.0f) > percent;
    }
}