#ifndef GLADOS_ARGUMENT_HPP
#define GLADOS_ARGUMENT_HPP

#include "math/Quat.h"
#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Vec4.h"
//#include "math/Mat4.hpp"

namespace GLaDOS {
    class Argument {
      public:
        Argument() = default;
        virtual ~Argument() = default;
        virtual void append(std::string& dest) const = 0;
    };

    template <typename T>
    class ArgumentType : public Argument {
      public:
        explicit ArgumentType(T const& type) : mData{type} {}
        void append(std::string& dest) const override {
            dest.append(StringUtils::normalize(mData));
        }

      private:
        T const& mData;
    };

    template <>
    class ArgumentType<bool> : public Argument {
      public:
        explicit ArgumentType(bool const& type) : mData{type} {}
        void append(std::string& dest) const override {
            dest.append(mData ? "true" : "false");
        }

      private:
        bool const& mData;
    };

    template <>
    class ArgumentType<Quat> : public Argument {
      public:
        explicit ArgumentType(Quat const& type) : mData{type} {}
        void append(std::string& dest) const override {
            std::stringstream sstream;
            sstream << mData.w << " + ";
            sstream << mData.x << "i + ";
            sstream << mData.y << "j + ";
            sstream << mData.z << "k";
            dest.append(sstream.str());
        }

      private:
        Quat const& mData;
    };

    template <>
    class ArgumentType<Vec4> : public Argument {
      public:
        explicit ArgumentType(Vec4 const& type) : mData{type} {}
        void append(std::string& dest) const override {
            std::stringstream sstream;
            sstream << "(x=" << mData.x << ", y=" << mData.y << ", z=" << mData.z << ", w=" << mData.w << ")";
            dest.append(sstream.str());
        }

      private:
        Vec4 const& mData;
    };

    template <>
    class ArgumentType<Vec3> : public Argument {
      public:
        explicit ArgumentType(Vec3 const& type) : mData{type} {}
        void append(std::string& dest) const override {
            std::stringstream sstream;
            sstream << "(x=" << mData.x << ", y=" << mData.y << ", z=" << mData.z << ")";
            dest.append(sstream.str());
        }

      private:
        Vec3 const& mData;
    };

    template <>
    class ArgumentType<Vec2> : public Argument {
      public:
        explicit ArgumentType(Vec2 const& type) : mData{type} {}
        void append(std::string& dest) const override {
            std::stringstream sstream;
            sstream << "(x=" << mData.x << ", y=" << mData.y << ")";
            dest.append(sstream.str());
        }

      private:
        Vec2 const& mData;
    };

//    template <>
//    class ArgumentType<Mat4<real>> : public Argument {
//      public:
//        explicit ArgumentType(Mat4<real> const& type) : mData{type} {}
//        void append(std::string& dest) const override {
//            std::stringstream sstream;
//            sstream << std::fixed << std::setprecision(5);
//
//            for (std::size_t i = 0; i < 4; i++) {
//                for (std::size_t j = 0; j < 4; j++) {
//                    sstream << mData._m44[i][j] << std::setw(5);
//                }
//                sstream << "\n";
//            }
//            dest.append(sstream.str());
//        }
//
//      private:
//        Mat4<real> const& mData;
//    };
}  // namespace GLaDOS

#endif  // GLADOS_ARGUMENT_HPP
