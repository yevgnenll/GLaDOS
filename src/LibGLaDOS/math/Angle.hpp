#ifndef GLADOS_ANGLE_HPP
#define GLADOS_ANGLE_HPP

#include <ratio>

#include "utils/Enumeration.h"

namespace GLaDOS {
  template <typename Ratio>
  class Angle {
  public:
    static constexpr auto ratio =
        static_cast<real>(Ratio::num) / static_cast<real>(Ratio::den);

    constexpr Angle() noexcept = default;
    explicit constexpr Angle(real r) noexcept : val{r} {}

    template <typename S>
    constexpr explicit Angle(const Angle<S>& r) noexcept : val{r.get() / r.ratio * ratio} {}

    [[nodiscard]] constexpr real get() const noexcept { return val; }
    explicit constexpr operator real() const noexcept { return val; }

    Angle operator+(const real& value) const {
      return Angle<Ratio>{val + value};
    }
    Angle operator-(const real& value) const {
      return Angle<Ratio>{val - value};
    }
    Angle operator*(const real& value) const {
      return Angle<Ratio>{val * value};
    }
    Angle operator/(const real& value) const {
      return Angle<Ratio>{val / value};
    }

  private:
    real val = 0;
  };

  using PiRatio = std::ratio<66627445592888887, 21208174623389167>;
  using RadRatio = std::ratio<PiRatio::num, PiRatio::den * 180>;
  using DegRatio = std::ratio<1>;
  using Rad = Angle<RadRatio>;
  using Deg = Angle<DegRatio>;

  constexpr Rad operator""_rad(unsigned long long value) noexcept { return Rad{static_cast<real>(value)}; }
  constexpr Rad operator""_rad(long double value) noexcept { return Rad{static_cast<real>(value)}; }

  constexpr Deg operator""_deg(unsigned long long value) noexcept { return Deg{static_cast<real>(value)}; }
  constexpr Deg operator""_deg(long double value) noexcept { return Deg{static_cast<real>(value)}; }
}  // namespace GLaDOS

#endif  //GLADOS_ANGLE_HPP
