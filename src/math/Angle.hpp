#ifndef GAMEENGINE_ANGLE_HPP
#define GAMEENGINE_ANGLE_HPP

#include <ratio>

#include "utils/Enumeration.h"

namespace GameEngine {
  template <typename Ratio>
  class Angle {
  public:
    static constexpr auto ratio =
        static_cast<real>(Ratio::num) / static_cast<real>(Ratio::den);

    constexpr Angle() noexcept = default;
    explicit constexpr Angle(float r) noexcept : val{r} {}

    template <typename S>
    constexpr explicit Angle(const Angle<S>& r) noexcept : val{r.get() / r.ratio * ratio} {}

    [[nodiscard]] constexpr float get() const noexcept { return val; }
    explicit constexpr operator real() const noexcept { return val; }

  private:
    real val = 0;
  };

  using PiRatio = std::ratio<66627445592888887, 21208174623389167>;
  using RadRatio = std::ratio<PiRatio::num, PiRatio::den * 180>;
  using DegRatio = std::ratio<1>;
  using Rad = Angle<RadRatio>;
  using Deg = Angle<DegRatio>;

  constexpr Rad operator""_rad(unsigned long long f) noexcept { return Rad{static_cast<real>(f)}; }
  constexpr Rad operator""_rad(long double f) noexcept { return Rad{static_cast<real>(f)}; }

  constexpr Deg operator""_deg(unsigned long long f) noexcept { return Deg{static_cast<real>(f)}; }
  constexpr Deg operator""_deg(long double f) noexcept { return Deg{static_cast<real>(f)}; }
}  // namespace GameEngine

#endif  //GAMEENGINE_ANGLE_HPP
