#ifndef GLADOS_UUID_H
#define GLADOS_UUID_H

#include "utils/Utility.h"
#include "math/Random.hpp"
#include <string>

namespace GLaDOS {
    class UUID {
      public:
        DISALLOW_COPY_AND_ASSIGN(UUID);

        static std::string generateV4();
    };
}

#endif  // GLADOS_UUID_H
