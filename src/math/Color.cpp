#include "Color.h"

#include "math/Math.h"
#include "utils/StringUtils.h"

namespace GLaDOS {
  Color::Color(real _r, real _g, real _b) : r{_r}, g{_g}, b{_b}, a{0.0} {}

  Color::Color(real _r, real _g, real _b, real _a) : r{_r}, g{_g}, b{_b}, a{_a} {}

  Color::Color(Color&& other) noexcept : Color{} {
    Color::swap(*this, other);
  }

  Color& Color::operator=(Color other) {
    // copy and swap idiom (effective c++ section 11)
    Color::swap(*this, other);
    return *this;
  }

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

  const real& Color::operator[](unsigned int i) const {
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

  Vec3 Color::toVec3() const {
    return Vec3{r, g, b};
  }

  Vec4 Color::toVec4() const {
    return Vec4{r, g, b, a};
  }

  Color Color::lerp(const Color& a, const Color& b, real t) {
    // c = a+(b-a)*t
    return Color{
        Math::lerp(a.r, b.r, t),
        Math::lerp(a.g, b.g, t),
        Math::lerp(a.b, b.b, t),
        Math::lerp(a.a, b.a, t)};
  }

  unsigned char Color::toByte(real value) {
    real f2 = Math::max(real(0.0), Math::min(real(1.0), value));
    return static_cast<unsigned char>(Math::floor(f2 == real(1.0) ? 255 : f2 * 256));
  }

  Color Color::fromRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return Color{r * invConverter, g * invConverter, b * invConverter, a * invConverter};
  }

  Color Color::fromRGB(uint32_t rgb, real a) {
    return Color{
        static_cast<real>(rgb >> 16 & 0xff) * invConverter,
        static_cast<real>(rgb >> 8 & 0xff) * invConverter,
        static_cast<real>(rgb & 0xff) * invConverter,
        a};
  }

  Color Color::fromHex(uint32_t hexValue) {
    return Color{((hexValue >> 16) & 0xff) * invConverter,
                 ((hexValue >> 8) & 0xff) * invConverter,
                 ((hexValue)&0xff) * invConverter,
                 ((hexValue >> 24) & 0xff) * invConverter};
  }

  Color Color::fromHex(const std::string& hexString) {
    int hexValue = StringUtils::toInt(hexString.substr(1, hexString.size() - 1), 16);
    return Color(((hexValue >> 16) & 0xff) * invConverter,
                 ((hexValue >> 8) & 0xff) * invConverter,
                 ((hexValue)&0xff) * invConverter,
                 real(1.0));
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

  Color Color::fromHSV(real hue, real saturation, real value, real alpha) {
    /*
    * H(Hue): 0 - 360 degree (int)
    * S(Saturation): 0 - 1.00 (real)
    * V(Value): 0 - 1.00 (real)
    */
    const real hDiv60 = hue / 60.0f;
    const real hDiv60Floor = Math::floor(hDiv60);
    const real hDiv60Fraction = hDiv60 - hDiv60Floor;

    const real rgbValues[4] = {
        value,
        value * (real(1.0) - saturation),
        value * (real(1.0) - hDiv60Fraction * saturation),
        value * (real(1.0) - (real(1.0) - hDiv60Fraction) * saturation)};
    constexpr int32_t rgbSwizzle[6][3] = {
        {0, 3, 1},
        {2, 0, 1},
        {1, 0, 3},
        {1, 2, 0},
        {3, 1, 0},
        {0, 1, 2}};
    const int32_t swizzleIndex = (int32_t)hDiv60Floor % 6;

    return Color{
        rgbValues[rgbSwizzle[swizzleIndex][0]],
        rgbValues[rgbSwizzle[swizzleIndex][1]],
        rgbValues[rgbSwizzle[swizzleIndex][2]],
        alpha};
  }

  Color Color::fromHSV(const Vec3& hsv, real alpha) {
    return fromHSV(hsv.x, hsv.y, hsv.z, alpha);
  }

  void Color::swap(Color& first, Color& second) {
    using std::swap;

    swap(first.r, second.r);
    swap(first.g, second.g);
    swap(first.b, second.b);
    swap(first.a, second.a);
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