#ifndef GLADOS_ANIMATIONCURVE_HPP
#define GLADOS_ANIMATIONCURVE_HPP

#include "utils/Stl.h"
#include "KeyFrame.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
    template <typename T, std::size_t N>
    class AnimationCurve {
      public:
        AnimationCurve() = default;
        ~AnimationCurve() = default;

        Vector<KeyFrame<N>> getKeyFrame() const;
        std::size_t length() const;

        int addKey(const KeyFrame<N>& key);
        void removeKey(int index);
        int moveKey(int index, const KeyFrame<N>& key);
        real evaluate(real time);

        static AnimationCurve<T, N> constant(real timeStart, real timeEnd, real value);
        static AnimationCurve<T, N> linear(real timeStart, real valueStart, real timeEnd, real valueEnd);
        static AnimationCurve<T, N> easeInOut(real timeStart, real valueStart, real timeEnd, real valueEnd);

      private:
        Vector<KeyFrame<N>> mKeys;
        AnimationWrapMode mWrapMode;
    };

    template <typename T, std::size_t N>
    Vector<KeyFrame<N>> AnimationCurve<T, N>::getKeyFrame() const {
        return mKeys;
    }

    template <typename T, std::size_t N>
    std::size_t AnimationCurve<T, N>::length() const {
        return 0;
    }

    template <typename T, std::size_t N>
    int AnimationCurve<T, N>::addKey(const KeyFrame<N>& key) {
        return 0;
    }

    template <typename T, std::size_t N>
    void AnimationCurve<T, N>::removeKey(int index) {
    }

    template <typename T, std::size_t N>
    int AnimationCurve<T, N>::moveKey(int index, const KeyFrame<N>& key) {
        return 0;
    }

    template <typename T, std::size_t N>
    real AnimationCurve<T, N>::evaluate(real time) {
        return 0;
    }

    template <typename T, std::size_t N>
    AnimationCurve<T, N> AnimationCurve<T, N>::constant(real timeStart, real timeEnd, real value) {
        return AnimationCurve();
    }

    template <typename T, std::size_t N>
    AnimationCurve<T, N> AnimationCurve<T, N>::linear(real timeStart, real valueStart, real timeEnd, real valueEnd) {
        return AnimationCurve();
    }

    template <typename T, std::size_t N>
    AnimationCurve<T, N> AnimationCurve<T, N>::easeInOut(real timeStart, real valueStart, real timeEnd, real valueEnd) {
        return AnimationCurve();
    }
}

#endif  // GLADOS_ANIMATIONCURVE_HPP
