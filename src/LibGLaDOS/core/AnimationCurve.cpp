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
        return 0;
    }

    int AnimationCurve::addKey(const KeyFrame& key) {
        // TODO
        return 0;
    }

    void AnimationCurve::removeKey(int index) {
        // TODO
    }

    int AnimationCurve::moveKey(int index, KeyFrame key) {
        // TODO
        return 0;
    }

    real AnimationCurve::evaluate(real time) {
        // TODO
        return 0.f;
    }

    AnimationCurve AnimationCurve::constant(real timeStart, real timeEnd, real value) {
        // TODO
        return AnimationCurve({});
    }

    AnimationCurve AnimationCurve::easeInOut(real timeStart, real valueStart, real timeEnd, real valueEnd) {
        // TODO
        return AnimationCurve({});
    }

    AnimationCurve AnimationCurve::linear(real timeStart, real valueStart, real timeEnd, real valueEnd) {
        // TODO
        return AnimationCurve({});
    }
}