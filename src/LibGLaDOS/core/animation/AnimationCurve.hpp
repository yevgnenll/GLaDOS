#ifndef GLADOS_ANIMATIONCURVE_HPP
#define GLADOS_ANIMATIONCURVE_HPP

#include "utils/Stl.h"
#include "KeyFrame.h"
#include "utils/Enumeration.h"
#include "math/Vec3.h"
#include "math/Quat.h"
#include "math/Math.h"

namespace GLaDOS {
    inline float adjustHermiteResult(real f) {
        return f;
    }

    inline Vec3 adjustHermiteResult(const Vec3& v) {
        return v;
    }

    inline Quat adjustHermiteResult(const Quat& q) {
        return Quat::normalize(q);
    }

    inline void neighborhood(const float& a, float& b) { }
    inline void neighborhood(const Vec3& a, Vec3& b) { }
    inline void neighborhood(const Quat& a, Quat& b) {
        if (Quat::dot(a, b) < 0) {
            b = -b;
        }
    }

    template <typename T, std::size_t N>
    class AnimationCurve {
      public:
        AnimationCurve() = default;
        AnimationCurve(std::initializer_list<KeyFrame<N>> initializerList);
        ~AnimationCurve() = default;

        std::size_t length() const;
        real getStartTime() const;
        real getEndTime() const;
        real getDuration() const;

        T evaluate(real time, bool loop, Interpolation interpolation) const;
        KeyFrame<N> operator[](std::size_t index) const;

         void addKeyFrame(const KeyFrame<N>& keyFrame); // add a new keyframe at the end
         bool removeKeyFrame(std::size_t index); // remove a keyframe at index
         int moveKeyFrame(std::size_t index, const KeyFrame<N>& keyFrame); // move a keyframe into the index

       private:
        real clampTimeInCurve(real time, bool loop) const;
        int getKeyFrameIndex(real time, bool loop) const;

        T constant(real time, bool loop) const;
        T linear(real time, bool loop) const;
        T cubic(real time, bool loop) const;

        T hermite(real t, const T& p1, const T& s1, const T& _p2, const T& s2) const;
        inline T cast(const real* value) const;

        Vector<KeyFrame<N>> mKeyFrames;
    };

    template <typename T, std::size_t N>
    AnimationCurve<T, N>::AnimationCurve(std::initializer_list<KeyFrame<N>> initializerList) {
        for (const auto& keyFrame : initializerList) {
            mKeyFrames.emplace_back(keyFrame);
        }
    }

    template <typename T, std::size_t N>
    std::size_t AnimationCurve<T, N>::length() const {
        return mKeyFrames.size();
    }

    template <typename T, std::size_t N>
    real AnimationCurve<T, N>::getStartTime() const {
        if (length() == 0) {
            return real(0);
        }
        return mKeyFrames[0].time;
    }

    template <typename T, std::size_t N>
    real AnimationCurve<T, N>::getEndTime() const {
        if (length() == 0) {
            return real(0);
        }
        return mKeyFrames[length() - 1].time;
    }

    template <typename T, std::size_t N>
    real AnimationCurve<T, N>::getDuration() const {
        return getEndTime() - getStartTime();
    }

    template <typename T, std::size_t N>
    T AnimationCurve<T, N>::evaluate(real time, bool loop, Interpolation interpolation) const {
        if (interpolation == Interpolation::Constant) {
            return constant(time, loop);
        }

        if (interpolation == Interpolation::Linear) {
            return linear(time, loop);
        }

        return cubic(time, loop);
    }

    template <typename T, std::size_t N>
    KeyFrame<N> AnimationCurve<T, N>::operator[](std::size_t index) const {
        return mKeyFrames[index];
    }

    template <typename T, std::size_t N>
    void AnimationCurve<T, N>::addKeyFrame(const KeyFrame<N>& keyFrame) {
        mKeyFrames.emplace_back(keyFrame);
    }

    template <typename T, std::size_t N>
    bool AnimationCurve<T, N>::removeKeyFrame(std::size_t index) {
        if (index < 0 || index > mKeyFrames.size() - 1) {
            return false;
        }
        mKeyFrames.erase(mKeyFrames.begin() + index);
        return true;
    }

    template <typename T, std::size_t N>
    int AnimationCurve<T, N>::moveKeyFrame(std::size_t index, const KeyFrame<N>& keyFrame) {
        if (index < 0 || index > mKeyFrames.size() - 1) {
            return -1;
        }
        // TODO
        return 0;
    }

    template <typename T, std::size_t N>
    real AnimationCurve<T, N>::clampTimeInCurve(real time, bool loop) const {
        if (length() <= 1) {
            return real(0);
        }

        real startTime = getStartTime();
        real endTime = getEndTime();
        real duration = getDuration();
        if (duration <= real(0)) {
            return real(0);
        }

        if (loop) {
            time = Math::mod(time - startTime, duration);
            if (time < real(0)) {
                time += duration;
            }
            time += startTime;
        } else {
            time = Math::clamp(time, startTime, endTime);
        }

        return time;
    }

    template <typename T, std::size_t N>
    int AnimationCurve<T, N>::getKeyFrameIndex(real time, bool loop) const {
        if (length() <= 1) {
            return -1;
        }

        real startTime = getStartTime();
        if (loop) {
            real duration = getDuration();
            time = Math::mod(time - startTime, duration);
            if (time < real(0)) {
                time += duration;
            }
            time += startTime;
        } else {
            if (time <= startTime) {
                return 0;
            }
            if (time >= mKeyFrames[length() - 2].time) {
                return length() - 2;
            }
        }

        for (std::size_t i = length() - 1; i >= 0; i--) {
            if (time >= mKeyFrames[i].time) {
                return i;
            }
        }

        return -1;
    }

    template <typename T, std::size_t N>
    T AnimationCurve<T, N>::constant(real time, bool loop) const {
        int index = getKeyFrameIndex(time, loop);
        if (index < 0 || index >= length()) {
            return T();
        }

        return cast(&mKeyFrames[index].value[0]);
    }

    template <typename T, std::size_t N>
    T AnimationCurve<T, N>::linear(real time, bool loop) const {
        int currentKeyFrameIndex = getKeyFrameIndex(time, loop);
        if (currentKeyFrameIndex < 0 || currentKeyFrameIndex >= length() - 1) {
            return T();
        }
        std::size_t nextKeyFrameIndex = static_cast<size_t>(currentKeyFrameIndex + 1);
        real currentTime = clampTimeInCurve(time, loop);
        real keyFrameDelta = mKeyFrames[nextKeyFrameIndex].time - mKeyFrames[currentKeyFrameIndex].time;
        if (keyFrameDelta < real(0)) {
            return T();
        }
        real sampleTime = (currentTime - mKeyFrames[currentKeyFrameIndex].time) / keyFrameDelta;

        T start = cast(mKeyFrames[currentKeyFrameIndex].value);
        T end = cast(mKeyFrames[nextKeyFrameIndex].value);

        return Math::lerpUnclamped(start, end, sampleTime);
    }

    template <typename T, std::size_t N>
    T AnimationCurve<T, N>::cubic(real time, bool loop) const {
        int currentKeyFrameIndex = getKeyFrameIndex(time, loop);
        if (currentKeyFrameIndex < 0 || currentKeyFrameIndex >= length() - 1) {
            return T();
        }
        std::size_t nextKeyFrameIndex = static_cast<size_t>(currentKeyFrameIndex + 1);
        real currentTime = clampTimeInCurve(time, loop);
        real keyFrameDelta = mKeyFrames[nextKeyFrameIndex].time - mKeyFrames[currentKeyFrameIndex].time;
        if (keyFrameDelta < real(0)) {
            return T();
        }
        real sampleTime = (currentTime - mKeyFrames[currentKeyFrameIndex].time) / keyFrameDelta;

        T point1 = cast(mKeyFrames[currentKeyFrameIndex].value);
        T slope1;
        std::size_t size = N * sizeof(real);
        // Note: the Out tangent of the left key frame is taken here, and the In tangent is not used. The Out and In here
        // As like as two peas of Delta T, it is exactly the same as T.
        // Moreover, the copy function is used here, not the Cast function, because for T of quaternion type
        // The Cast function will normalize, and here is the tangent, so normalization is not required
        memcpy(&slope1, mKeyFrames[currentKeyFrameIndex].outTangent, size);
        slope1 = slope1 * keyFrameDelta;

        T point2 = cast(mKeyFrames[nextKeyFrameIndex].value);
        T slope2;
        // Note: the In tangent of the right key frame is taken here, and the Out tangent is not used
        memcpy(&slope2, mKeyFrames[nextKeyFrameIndex].inTangent, size);
        slope2 = slope2 * keyFrameDelta;

        return hermite(sampleTime, point1, slope1, point2, slope2);
    }

    template <typename T, std::size_t N>
    T AnimationCurve<T, N>::hermite(real t, const T& p1, const T& s1, const T& _p2, const T& s2) const {
        real tt = t * t;
        real ttt = tt * t;

        T p2 = _p2;
        neighborhood(p1, p2);

        real h1 = 2.0f * ttt - 3.0f * tt + 1.0f;
        real h2 = -2.0f * ttt + 3.0f * tt;
        real h3 = ttt - 2.0f * tt + t;
        real h4 = ttt - tt;

        T result = p1 * h1 + p2 * h2 + s1 * h3 + s2 * h4;
        return adjustHermiteResult(result);
    }

    template<>
    inline real AnimationCurve<real, 1>::cast(const real* value) const {
        return value[0];
    }

    template<>
    inline Vec2 AnimationCurve<Vec2, 2>::cast(const real* value) const {
        return Vec2{value[0], value[1]};
    }

    template<>
    inline Vec3 AnimationCurve<Vec3, 3>::cast(const real* value) const {
        return Vec3{value[0], value[1], value[2]};
    }

    template<>
    inline Quat AnimationCurve<Quat, 4>::cast(const real* value) const {
        return Quat{value[0], value[1], value[2], value[3]};
    }

    typedef AnimationCurve<real, 1> ScalarCurve;
    typedef AnimationCurve<Vec2, 2> Vec2Curve;
    typedef AnimationCurve<Vec3, 3> Vec3Curve;
    typedef AnimationCurve<Quat, 4> QuatCurve;
}

#endif  // GLADOS_ANIMATIONCURVE_HPP
