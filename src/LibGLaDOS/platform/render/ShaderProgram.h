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
    class Uniform;
    class Color;
    class Vec2;
    class Vec3;
    class Vec4;
    class DepthStencilState;
    struct DepthStencilDescription;
    class RasterizerState;
    struct RasterizerDescription;
    class ShaderProgram : public Resource {
      public:
        ShaderProgram();
        ~ShaderProgram() override;

        void setUniform(const std::string& name, int value);
        void setUniform(const std::string& name, unsigned int value);
        void setUniform(const std::string& name, float value);
        void setUniform(const std::string& name, const Vec2& value);
        void setUniform(const std::string& name, const Vec3& value);
        void setUniform(const std::string& name, const Vec4& value);
        void setUniform(const std::string& name, const Color& value);
        void setUniform(const std::string& name, const float* values, int count);
        void setUniform(const std::string& name, const Vec2* values, int count);
        void setUniform(const std::string& name, const Vec3* values, int count);
        void setUniform(const std::string& name, const Vec4* values, int count);
        void setUniform(const std::string& name, const Color* values, int count);
        void setUniform(const std::string& name, const Mat4<real>& value);
        void setUniform(const std::string& name, bool value);
        Uniform* getUniform(const std::string& name);
        Map<std::string, Uniform*>& getUniforms();
        std::size_t uniformSize() const;
        bool isValid() const;
        DepthStencilState* depthStencilState();
        void setDepthStencilState(const DepthStencilDescription& desc);
        RasterizerState* rasterizerState();
        void setRasterizerState(const RasterizerDescription& desc);
        bool exists(const std::string& name);

      private:
        virtual bool createShaderProgram(const std::string& vertex, const std::string& fragment) = 0;

        static Logger* logger;

      protected:
        void reserveUniformMemory();

        std::string mVertexShaderCode;
        std::string mFragmentShaderCode;
        Map<std::string, Uniform*> mUniforms;
        DepthStencilState* mDepthStencilState{nullptr};
        RasterizerState* mRasterizerState{nullptr};
        bool mIsValid{false};
        Blob mVertexUniformBuffer;
        Blob mFragmentUniformBuffer;
    };
}  // namespace GLaDOS

#endif
