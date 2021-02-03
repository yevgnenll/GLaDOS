#ifndef GLADOS_COLOR_H
#define GLADOS_COLOR_H

#include <string>

#include "utils/Enumeration.h"

namespace GLaDOS {
  class Color {
  public:
    Color() = default;
    Color(real _r, real _g, real _b);
    Color(real _r, real _g, real _b, real _a);

    bool operator==(const Color& rhs) const;
    bool operator!=(const Color& rhs) const;
    float& operator[](unsigned int i);

    static Color lerp(const Color& a, const Color& b, float t);
    static unsigned char toByte(real value);
    static Color fromRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    static Color fromHex(unsigned int hexValue);
    static Color fromHex(const std::string& hexString);
    static Color fromCMYK(real c, real m, real y, real k);
    static Color fromHSV(int hueInDegree, real saturation, real value);

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

    // Red, green, blue, alpha
    real r{1.0}, g{1.0}, b{1.0}, a{1.0};

  private:
    static constexpr real invConverter = real(1) / real(255);
  };
}  // namespace GLaDOS

#endif  //GLADOS_COLOR_H
