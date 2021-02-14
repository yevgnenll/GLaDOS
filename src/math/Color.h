#ifndef GLADOS_COLOR_H
#define GLADOS_COLOR_H

#include <string>

#include "Vec3.h"
#include "Vec4.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
  class Color {
  public:
    Color() = default;
    Color(real _r, real _g, real _b);
    Color(real _r, real _g, real _b, real _a);

    bool operator==(const Color& rhs) const;
    bool operator!=(const Color& rhs) const;
    real& operator[](unsigned int i);

    Vec3 toVec3() const;
    Vec4 toVec4() const;

    static Color lerp(const Color& a, const Color& b, float t);
    static unsigned char toByte(real value);
    static Color fromRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    static Color fromRGB(uint32_t rgb, real a = real(1.0));
    static Color fromHex(uint32_t hexValue);
    static Color fromHex(const std::string& hexString);
    static Color fromCMYK(real c, real m, real y, real k);
    static Color fromHSV(real hue, real saturation, real value, real alpha = real(1.0));
    static Color fromHSV(const Vec3& hsv, real alpha = real(1.0));

    static Color white;
    static Color grey;
    static Color black;
    static Color red;
    static Color green;
    static Color blue;
    static Color yellow;
    static Color orange;
    static Color purple;
    static Color GizmoRed;
    static Color GizmoGreen;
    static Color GizmoBlue;

    real r{1.0}, g{1.0}, b{1.0}, a{1.0};

  private:
    static constexpr real invConverter = real(1) / real(255);
  };
}  // namespace GLaDOS

#endif  //GLADOS_COLOR_H
