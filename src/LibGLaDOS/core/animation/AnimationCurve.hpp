#ifndef GLADOS_ANIMATIONCURVE_HPP
#define GLADOS_ANIMATIONCURVE_HPP

#include "utils/Stl.h"
#include "KeyFrame.h"
#include "utils/Enumeration.h"
#include "math/Vec3.h"
#include "math/Quat.h"
#include "math/Math.h"

namespace GLaDOS {
    template <typename T, std::size_t N>
    class AnimationCurve {
      public:
        AnimationCurve() = default;
        AnimationCurve(std::initializer_list<KeyFrame<N>> initializerList);
        ~AnimationCurve() = default;

        void resize(std::size_t size);
        std::size_t length() const;
        Interpolation getInterpolation() const;
        void setInterpolation(Interpolation interpolation);
        real getStartTime() const;
        real getEndTime() const;
        real getDuration() const;
        T evaluate(real time, bool loop);
        KeyFrame<N> operator[](std::size_t index) const;

         void addKeyFrame(const KeyFrame<N>& keyFrame); // add a new keyframe at the end
         bool removeKeyFrame(std::size_t index); // remove a keyframe at index
         int moveKeyFrame(std::size_t index, const KeyFrame<N>& keyFrame); // move a keyframe into the index

      protected:
        real clampTimeInCurve(real time, bool loop);
        int getKeyFrameIndex(real time, bool loop) const;

        T constant(real time, bool loop);
        T linear(real time, bool loop);
        T cubic(real time, bool loop);

        inline T cast(real* value);

      private:
        Vector<KeyFrame<N>> mKeyFrames;
        Interpolation mInterpolation{Interpolation::Linear};
    };

    template <typename T, std::size_t N>
    AnimationCurve<T, N>::AnimationCurve(std::initializer_list<KeyFrame<N>> initializerList) {
        for (const auto& keyFrame : initializerList) {
            mKeyFrames.template emplace_back(keyFrame);
        }
    }

    template <typename T, std::size_t N>
    void AnimationCurve<T, N>::resize(std::size_t size) {
        mKeyFrames.resize(size);
    }

    template <typename T, std::size_t N>
    std::size_t AnimationCurve<T, N>::length() const {
        return mKeyFrames.size();
    }

    template <typename T, std::size_t N>
    Interpolation AnimationCurve<T, N>::getInterpolation() const {
        return mInterpolation;
    }

    template <typename T, std::size_t N>
    void AnimationCurve<T, N>::setInterpolation(Interpolation interpolation) {
        mInterpolation = interpolation;
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
    T AnimationCurve<T, N>::evaluate(real time, bool loop) {
        if (mInterpolation == Interpolation::Constant) {
            return constant(time, loop);
        }

        if (mInterpolation == Interpolation::Linear) {
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
    real AnimationCurve<T, N>::clampTimeInCurve(real time, bool loop) {
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
    T AnimationCurve<T, N>::constant(real time, bool loop) {
        int index = getKeyFrameIndex(time, loop);
        if (index < 0 || index >= length()) {
            return T();
        }

        return cast(&mKeyFrames[index].value[0]);
    }

    template <typename T, std::size_t N>
    T AnimationCurve<T, N>::linear(real time, bool loop) {
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

        T start = cast(&mKeyFrames[currentKeyFrameIndex].value[0]);
        T end = cast(&mKeyFrames[nextKeyFrameIndex].value[0]);

        return Math::lerpUnclamped(start, end, sampleTime);
    }

    template <typename T, std::size_t N>
    T AnimationCurve<T, N>::cubic(real time, bool loop) {
        return T(); // TODO
    }

    template<>
    inline real AnimationCurve<real, 1>::cast(real* value) {
        return value[0];
    }

    template<>
    inline Vec2 AnimationCurve<Vec2, 2>::cast(real* value) {
        return Vec2{value[0], value[1]};
    }

    template<>
    inline Vec3 AnimationCurve<Vec3, 3>::cast(real* value) {
        return Vec3{value[0], value[1], value[2]};
    }

    template<>
    inline Quat AnimationCurve<Quat, 4>::cast(real* value) {
        return Quat{value[0], value[1], value[2], value[3]};
    }

    typedef AnimationCurve<real, 1> ScalarCurve;
    typedef AnimationCurve<Vec2, 2> Vec2Curve;
    typedef AnimationCurve<Vec3, 3> Vec3Curve;
    typedef AnimationCurve<Quat, 4> QuatCurve;
}

#endif  // GLADOS_ANIMATIONCURVE_HPP
