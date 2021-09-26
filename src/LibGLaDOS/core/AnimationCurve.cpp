#include "AnimationCurve.h"

namespace GLaDOS {
    AnimationCurve::AnimationCurve(const Vector<KeyFrame>& keys) {
        // TODO
    }

    AnimationCurve::~AnimationCurve() {

    }

    Vector<KeyFrame> AnimationCurve::getKeyFrame() const {
        return mKeys;
    }

    std::size_t AnimationCurve::length() const {
        return mKeys.size();
    }

    int AnimationCurve::addKey(real time, real value) {
        // TODO
    }

    int AnimationCurve::addKey(const KeyFrame& key) {
        // TODO
    }

    void AnimationCurve::removeKey(int index) {
        // TODO
    }

    int AnimationCurve::moveKey(int index, KeyFrame key) {
        // TODO
    }

    real AnimationCurve::evaluate(real time) {
        // TODO
    }

    AnimationCurve AnimationCurve::constant(real timeStart, real timeEnd, real value) {
        // TODO
    }

    AnimationCurve AnimationCurve::easeInOut(real timeStart, real valueStart, real timeEnd, real valueEnd) {
        // TODO
    }

    AnimationCurve AnimationCurve::linear(real timeStart, real valueStart, real timeEnd, real valueEnd) {
        // TODO
    }
}