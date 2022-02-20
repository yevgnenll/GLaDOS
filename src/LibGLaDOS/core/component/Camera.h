#ifndef GLADOS_CAMERA_H
#define GLADOS_CAMERA_H

#include "core/Component.h"
#include "math/Mat4.hpp"
#include "math/Ray.h"
#include "math/Rect.hpp"
#include "memory/BitMask.h"

namespace GLaDOS {
    class RenderTexture;
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
        bool isOrthographic() const;
        void setOrthographic(bool orthographic);

        Deg fieldOfView() const;
        real nearClipPlane() const;
        real farClipPlane() const;
        BitMask* cullingMask();
        Rect<real> getViewportRect() const;
        void setViewportRect(Rect<real> viewport);
        void setFieldOfView(Deg fov);
        void setNearClipPlane(real near);
        void setFarClipPlane(real far);
        void setUnitSize(real unitSize);
        real getUnitSize() const;
        void setRenderTexture(RenderTexture* renderTexture);
        RenderTexture* getRenderTexture();

        static real aspectRatio();

      protected:
        void fixedUpdate(real fixedDeltaTime) override;
        void update(real deltaTime) override;
        void render() override;
        Component* clone() override;

      private:
        Deg mFieldOfView{real(60.0)};
        real mNearClipPlane{real(0.1)};
        real mFarClipPlane{real(1000.0)};
        bool mIsOrthographic{false};
        real mUnitSize{real(1)}; // only affect orthographic projection
        Rect<real> mViewportRect; // normalized viewport Rect default is (0, 0, 1, 1) bottom-left to top-right
        RenderTexture* mRenderTarget{nullptr};
        BitMask mCullingMask;
    };
}  // namespace GLaDOS

#endif  //GLADOS_CAMERA_H
