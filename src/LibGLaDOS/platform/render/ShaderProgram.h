#ifndef GLADOS_SHADERPROGRAM_H
#define GLADOS_SHADERPROGRAM_H

#include <string>

#include "resource/Resource.h"
#include "utils/Utility.h"
#include "memory/Blob.h"

namespace GLaDOS {
    class Logger;
    template <typename T>
    class Mat4;
    template <typename T>
    class Point;
    template <typename T>
    class Size;
    template <typename T>
    class Rect;
    class Uniform;
    class Color;
    class Vec2;
    class Vec3;
    class Vec4;
    class DepthStencilState;
    struct DepthStencilDescription;
    class RasterizerState;
    struct RasterizerDescription;
    class Shader;
    class ShaderProgram {
      public:
        ShaderProgram() = default;
        virtual ~ShaderProgram();

        void setUniform(const std::string& name, int value);
        void setUniform(const std::string& name, unsigned int value);
        void setUniform(const std::string& name, float value);
        void setUniform(const std::string& name, const Vec2& value);
        void setUniform(const std::string& name, const Point<real>& value);
        void setUniform(const std::string& name, const Vec3& value);
        void setUniform(const std::string& name, const Vec4& value);
        void setUniform(const std::string& name, const Color& value);
        void setUniform(const std::string& name, const Point<int32_t>& value);
        void setUniform(const std::string& name, const Size<int32_t>& value);
        void setUniform(const std::string& name, const Rect<int32_t>& value);
        void setUniform(const std::string& name, const Point<uint32_t>& value);
        void setUniform(const std::string& name, const Size<uint32_t>& value);
        void setUniform(const std::string& name, const Rect<uint32_t>& value);
        void setUniform(const std::string& name, float* values, std::size_t count);
        void setUniform(const std::string& name, Vec2* values, std::size_t count);
        void setUniform(const std::string& name, Vec3* values, std::size_t count);
        void setUniform(const std::string& name, Vec4* values, std::size_t count);
        void setUniform(const std::string& name, Color* values, std::size_t count);
        void setUniform(const std::string& name, Mat4<real>* values, std::size_t count);
        void setUniform(const std::string& name, const Mat4<real>& value);
        void setUniform(const std::string& name, bool value);

        bool addUniform(const std::string& name, Uniform* uniform);
        Uniform* getUniform(const std::string& name);
        UnorderedMap<std::string, Uniform*>& getUniforms();
        std::size_t uniformSize() const;

        bool isValid() const;

        DepthStencilState* depthStencilState();
        void setDepthStencilState(const DepthStencilDescription& desc);
        RasterizerState* rasterizerState();
        void setRasterizerState(const RasterizerDescription& desc);

        Shader* getVertexShader();
        Shader* getFragmentShader();

      private:
        virtual bool createShaderProgram(Shader* vertex, Shader* fragment) = 0;

      protected:
        void reserveUniformMemory();

        static Logger* logger;
        UnorderedMap<std::string, Uniform*> mUniforms;
        DepthStencilState* mDepthStencilState{nullptr};
        RasterizerState* mRasterizerState{nullptr};
        bool mIsValid{false};
        Blob mVertexUniformBuffer;
        Blob mFragmentUniformBuffer;
        Shader* mVertexShader{nullptr};
        Shader* mFragmentShader{nullptr};
    };
}  // namespace GLaDOS

#endif
