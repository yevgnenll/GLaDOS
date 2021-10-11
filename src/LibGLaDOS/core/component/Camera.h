#ifndef GLADOS_CAMERA_H
#define GLADOS_CAMERA_H

#include "core/Component.h"
#include "math/Mat4.hpp"
#include "math/Ray.h"
#include "math/Rect.hpp"

namespace GLaDOS {
    class Camera : public Component {
      public:
        Camera();
        explicit Camera(const Vec3& pos);
        ~Camera() override = default;

        Mat4<real> projectionMatrix() const;
        Mat4<real> worldToCameraMatrix() const;
        Mat4<real> cameraToWorldMatrix() const;

        Ray screenPointToRay(const Vec3& pos);
        Vec3 screenToWorldPoint(const Vec2& pos);
        Vec2 worldToScreenPoint(const Vec3& pos);
        void setOrthographic(bool orthographic);

        real fieldOfView() const;
        real nearClipPlane() const;
        real farClipPlane() const;
        real aspectRatio() const;
        Rect<uint32_t> viewportRect() const;
        void setFieldOfView(real fov);
        void setNearClipPlane(real near);
        void setFarClipPlane(real far);

      protected:
        void update(real deltaTime) override;
        void render() override;

      private:
        real mFieldOfView{real(60.0)};
        real mNearClipPlane{real(0.1)};
        real mFarClipPlane{real(1000.0)};
        bool mIsOrthographic{false};
        Rect<uint32_t> mViewportRect;
    };
}  // namespace GLaDOS

#endif  //GLADOS_CAMERA_H
