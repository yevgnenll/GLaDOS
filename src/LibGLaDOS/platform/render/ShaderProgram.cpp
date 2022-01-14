#include "ShaderProgram.h"
#include "RenderState.h"
#include "Renderer.h"
#include "Uniform.h"
#include "math/Color.h"
#include "platform/Platform.h"

namespace GLaDOS {
    Logger* ShaderProgram::logger = LoggerRegistry::getInstance().makeAndGetLogger("ShaderProgram");
    ShaderProgram::~ShaderProgram() {
        deallocValueInMap(mUniforms);
        DELETE_T(mDepthStencilState, DepthStencilState);
        DELETE_T(mRasterizerState, RasterizerState);
    }

    void ShaderProgram::setUniform(const std::string& name, int value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << value;
    }

    void ShaderProgram::setUniform(const std::string& name, unsigned int value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << value;
    }

    void ShaderProgram::setUniform(const std::string& name, float value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << value;
    }

    void ShaderProgram::setUniform(const std::string& name, const Vec2& value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << const_cast<Vec2&>(value);
    }

    void ShaderProgram::setUniform(const std::string& name, const Point<real>& value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << const_cast<Point<real>&>(value);
    }

    void ShaderProgram::setUniform(const std::string& name, const Vec3& value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << const_cast<Vec3&>(value);
    }

    void ShaderProgram::setUniform(const std::string& name, const Vec4& value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << const_cast<Vec4&>(value);
    }

    void ShaderProgram::setUniform(const std::string& name, const Color& value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << const_cast<Color&>(value);
    }

    void ShaderProgram::setUniform(const std::string& name, const Point<int32_t>& value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << const_cast<Point<int32_t>&>(value);
    }

    void ShaderProgram::setUniform(const std::string& name, const Size<int32_t>& value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << const_cast<Size<int32_t>&>(value);
    }

    void ShaderProgram::setUniform(const std::string& name, const Rect<int32_t>& value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << const_cast<Rect<int32_t>&>(value);
    }

    void ShaderProgram::setUniform(const std::string& name, const Point<uint32_t>& value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << const_cast<Point<uint32_t>&>(value);
    }

    void ShaderProgram::setUniform(const std::string& name, const Size<uint32_t>& value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << const_cast<Size<uint32_t>&>(value);
    }

    void ShaderProgram::setUniform(const std::string& name, const Rect<uint32_t>& value) {
        auto it = mUniforms.find(name);
        if (it == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*it->second) << const_cast<Rect<uint32_t>&>(value);
    }

    void ShaderProgram::setUniform(const std::string& name, float* values, int count) {
        auto iter = mUniforms.find(name);
        if (iter == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        for (int i = 0; i < count; ++i) {
            (*iter->second) << values[i];
        }
    }

    void ShaderProgram::setUniform(const std::string& name, Vec2* values, int count) {
        auto iter = mUniforms.find(name);
        if (iter == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        for (int i = 0; i < count; ++i) {
            (*iter->second) << values[i];
        }
    }

    void ShaderProgram::setUniform(const std::string& name, Vec3* values, int count) {
        auto iter = mUniforms.find(name);
        if (iter == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        for (int i = 0; i < count; ++i) {
            (*iter->second) << values[i];
        }
    }

    void ShaderProgram::setUniform(const std::string& name, Vec4* values, int count) {
        auto iter = mUniforms.find(name);
        if (iter == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        for (int i = 0; i < count; ++i) {
            (*iter->second) << values[i];
        }
    }

    void ShaderProgram::setUniform(const std::string& name, Color* values, int count) {
        auto iter = mUniforms.find(name);
        if (iter == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        for (int i = 0; i < count; ++i) {
            (*iter->second) << values[i];
        }
    }

    void ShaderProgram::setUniform(const std::string& name, const Mat4<real>& value) {
        auto iter = mUniforms.find(name);
        if (iter == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*iter->second) << const_cast<Mat4<real>&>(value);
    }

    void ShaderProgram::setUniform(const std::string& name, bool value) {
        auto iter = mUniforms.find(name);
        if (iter == mUniforms.end()) {
            LOG_WARN(logger, "uniform {0} not exist", name);
            return;
        }

        (*iter->second) << static_cast<int8_t>(value);
    }

    bool ShaderProgram::addUniform(const std::string& name, Uniform* uniform) {
        if (uniform == nullptr) {
            return false;
        }

        auto result = mUniforms.try_emplace(uniform->mName, uniform);
        if (!result.second) {
            return false;
        }

        LOG_TRACE(logger, "Uniform add [{0}]", uniform->toString());
        return true;
    }

    Uniform* ShaderProgram::getUniform(const std::string& name) {
        auto iter = mUniforms.find(name);
        if (iter == mUniforms.end()) {
            return nullptr;
        }

        return iter->second;
    }

    Map<std::string, Uniform*>& ShaderProgram::getUniforms() {
        return mUniforms;
    }

    std::size_t ShaderProgram::uniformSize() const {
        return mUniforms.size();
    }

    bool ShaderProgram::isValid() const {
        return mIsValid;
    }

    DepthStencilState* ShaderProgram::depthStencilState() {
        if (mDepthStencilState == nullptr) {
            DepthStencilDescription defaultDesc{};
            setDepthStencilState(defaultDesc);
        }
        return mDepthStencilState;
    }

    void ShaderProgram::setDepthStencilState(const DepthStencilDescription& desc) {
        if (mDepthStencilState != nullptr) {
            DELETE_T(mDepthStencilState, DepthStencilState);
        }
        mDepthStencilState = Platform::getRenderer().createDepthStencilState(desc);
    }

    RasterizerState* ShaderProgram::rasterizerState() {
        if (mRasterizerState == nullptr) {
            RasterizerDescription defaultDesc{};
            setRasterizerState(defaultDesc);
        }
        return mRasterizerState;
    }

    void ShaderProgram::setRasterizerState(const RasterizerDescription& desc) {
        if (mRasterizerState != nullptr) {
            DELETE_T(mRasterizerState, RasterizerState);
        }
        mRasterizerState = Platform::getRenderer().createRasterizerState(desc);
    }

    void ShaderProgram::reserveUniformMemory() {
        // 모든 유니폼객체에 대해 개별버퍼를 생성하지 않고 Blob으로 셰이더에 전달하기 위해 메모리 배열을 위한 공간을 만든다.
        mVertexUniformBuffer.clear();
        mFragmentUniformBuffer.clear();

        std::size_t vertexUniformSize = 0;
        std::size_t fragmentUniformSize = 0;
        for (const auto& [key, uniform] : mUniforms) {
            if (!uniform->isUniformType()) {
                continue;
            }

            switch (uniform->mShaderType) {
                case ShaderType::VertexShader:
                    vertexUniformSize += (uniform->mOffset + uniform->size());
                    break;
                case ShaderType::FragmentShader:
                    fragmentUniformSize += (uniform->mOffset + uniform->size());
                    break;
                default:
                    LOG_WARN(logger, "Not supported type yet!");
                    break;
            }
        }

        mVertexUniformBuffer.resize(vertexUniformSize);
        mFragmentUniformBuffer.resize(fragmentUniformSize);
    }
}  // namespace GLaDOS