#include "InputHandler.h"
#include "math/Math.h"

namespace GLaDOS {
  InputHandler::InputHandler(KeyCode positive, KeyCode negative, real delta)
    : mPositive(positive), mNegative(negative), mDeltaSpeed(delta) {

  }

  KeyCode InputHandler::positive() const {
    return mPositive;
  }

  KeyCode InputHandler::negative() const {
    return mNegative;
  }

  real InputHandler::accumulator() const {
    return mAccumulator;
  }

  void InputHandler::accumulate(real delta) {
    mAccumulator = Math::clamp(mAccumulator + delta, real(-1.0), real(1.0));
  }

  void InputHandler::reset() {
    mAccumulator = real(0.0);
  }

  real InputHandler::delta() const {
    return mDeltaSpeed;
  }
}