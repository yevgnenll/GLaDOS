#include "UUID.h"
#include <sstream>

namespace GLaDOS {
    std::string UUID::generateV4() {
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