#include "UUID.h"
#include <sstream>

namespace GLaDOS {
    std::string UUID::generateV4() {
        // Honestly, this is not a real Unique ID but rather a pseudo UUID.
        // see how std::mt19937 random number generation works in documentation.
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << Random::nextInt(0, 15);
        }
        ss << "-";
        for (i = 0; i < 4; i++) {
            ss << Random::nextInt(0, 15);
        }
        ss << "-4";
        for (i = 0; i < 3; i++) {
            ss << Random::nextInt(0, 15);
        }
        ss << "-";
        ss << Random::nextInt(8, 11);
        for (i = 0; i < 3; i++) {
            ss << Random::nextInt(0, 15);
        }
        ss << "-";
        for (i = 0; i < 12; i++) {
            ss << Random::nextInt(0, 15);
        };
        return ss.str();
    }
}