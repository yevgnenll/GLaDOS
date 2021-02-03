#include "Color.h"

#include "math/Math.h"
#include "utils/StringUtils.h"

namespace GLaDOS {
  Color::Color(real _r, real _g, real _b) : r{_r}, g{_g}, b{_b}, a{0.0} {}

  Color::Color(real _r, real _g, real _b, real _a) : r{_r}, g{_g}, b{_b}, a{_a} {}

  bool Color::operator==(const Color& rhs) const {
    return Math::equal(r, rhs.r) && Math::equal(g, rhs.g) && Math::equal(b, rhs.b) && Math::equal(a, rhs.a);
  }

  bool Color::operator!=(const Color& rhs) const { return !(*this == rhs); }

  real& Color::operator[](unsigned int i) {
    switch (i) {
      case 0:
        return r;
      case 1:
        return g;
      case 2:
        return b;
      default:
        return a;
    }
  }

  Color Color::lerp(const Color& a, const Color& b, real t) {
    // c = a+(b-a)*t
    return Color{Math::max(real(0.0), Math::min(real(1.0), a.r + (b.r - a.r) * t)),
                 Math::max(real(0.0), Math::min(real(1.0), a.g + (b.g - a.g) * t)),
                 Math::max(real(0.0), Math::min(real(1.0), a.b + (b.b - a.b) * t)),
                 Math::max(real(0.0), Math::min(real(1.0), a.a + (b.a - a.a) * t))};
  }

  unsigned char Color::toByte(real value) {
    real f2 = Math::max(real(0.0), Math::min(real(1.0), value));
    return static_cast<unsigned char>(Math::floor(f2 == real(1.0) ? 255 : f2 * 256));
  }

  Color Color::fromRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    return Color{r * invConverter, g * invConverter, b * invConverter, a * invConverter};
  }

  Color Color::fromHex(unsigned int hexValue) {
    return Color{((hexValue >> 16) & 0xFF) * invConverter,  // Extract the R byte
                 ((hexValue >> 8) & 0xFF) * invConverter,  // Extract the G byte
                 ((hexValue)&0xFF) * invConverter,  // Extract the B byte
                 ((hexValue >> 24) & 0xFF) * invConverter};  // Extract the A byte
  }

  Color Color::fromHex(const std::string& hexString) {
    int hexValue = StringUtils::toInt(hexString.substr(1, hexString.size() - 1), 16);
    return Color(((hexValue >> 16) & 0xFF) * invConverter,  // Extract the R byte
                 ((hexValue >> 8) & 0xFF) * invConverter,  // Extract the G byte
                 ((hexValue)&0xFF) * invConverter,  // Extract the B byte
                 real(1.0));  // alpha is fixed to 1
  }

  Color Color::fromCMYK(real c, real m, real y, real k) {
    constexpr auto hundred = real(100);
    static constexpr real cmykScale = real(1.0) / hundred;
    real k2 = Math::min(hundred, k);
    real precomputed = real(1.0) - k2 * cmykScale;

    return Color{
        (real(1.0) - Math::min(hundred, c) * cmykScale) * precomputed,
        (real(1.0) - Math::min(hundred, m) * cmykScale) * precomputed,
        (real(1.0) - Math::min(hundred, y) * cmykScale) * precomputed,
        real(1.0)};
  }

  Color Color::fromHSV(int hueInDegree, real saturation, real value) {
    /*
    * H(Hue): 0 - 360 degree (int)
    * S(Saturation): 0 - 1.00 (real)
    * V(Value): 0 - 1.00 (real)
    */
    // TODO
    return Color();
  }

  Color Color::white = Color(1., 1.0, 1.0, 1.0);
  Color Color::grey = Color(0.5, 0.5, 0.5, 1.0);
  Color Color::black = Color(0.0, 0.0, 0.0, 1.0);
  Color Color::red = Color(1.0, 0.0, 0.0, 1.0);
  Color Color::green = Color(0.0, 1.0, 0.0, 1.0);
  Color Color::blue = Color(0.0, 0.0, 1.0, 1.0);
  Color Color::yellow = Color(1.0, 1.0, 0.0, 1.0);
  Color Color::orange = Color(1.0, 0.5, 0.0, 1.0);
  Color Color::purple = Color(0.5, 0.0, 1.0, 1.0);
  Color Color::GizmoRed = Color(0.65, 0.2, 0.11, 1.0);
  Color Color::GizmoGreen = Color(0.47, 0.8, 0.29, 1.0);
  Color Color::GizmoBlue = Color(0.2, 0.39, 0.84, 1.0);
}  // namespace GLaDOS