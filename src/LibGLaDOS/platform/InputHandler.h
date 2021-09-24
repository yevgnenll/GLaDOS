#ifndef GLADOS_INPUTHANDLER_H
#define GLADOS_INPUTHANDLER_H

#include "KeyCode.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
    class InputHandler {
      public:
        InputHandler(KeyCode positive, KeyCode negative, real delta);
        ~InputHandler() = default;

        KeyCode positive() const;
        KeyCode negative() const;
        real accumulator() const;
        void accumulate(real delta);
        void reset();
        real delta() const;

      private:
        KeyCode mPositive;
        KeyCode mNegative;
        real mAccumulator{0};
        real mDeltaSpeed{0};
    };
}  // namespace GLaDOS

#endif  //GLADOS_INPUTHANDLER_H
