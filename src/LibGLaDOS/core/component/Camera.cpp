#include "Camera.h"

#include "Transform.h"
#include "core/GameObject.hpp"
#include "math/UVec3.h"
#include "math/Vec2.h"
#include "platform/Platform.h"
#include "platform/OSTypes.h"

namespace GLaDOS {
    Camera::Camera() : Component{"Camera"} {
    }

    Camera::Camera(const Vec3& pos) : Component{"Camera"} {
        mGameObject->transform()->setLocalPosition(pos);
    }

    Mat4<real> Camera::projectionMatrix() const {
        if (mIsOrthographic) {
            // left, right, bottom, top
            real halfWidth = static_cast<real>(Platform::getInstance().getDrawableWidth()) * 0.5f * mUnitSize;
            real halfHeight = static_cast<real>(Platform::getInstance().getDrawableHeight()) * 0.5f * mUnitSize;
            return Mat4<real>::orthogonal(-halfWidth, halfWidth, -halfHeight, halfHeight, mNearClipPlane, mFarClipPlane);
        }
        return Mat4<real>::perspective(Math::toRadians(fieldOfView()), aspectRatio(), mNearClipPlane, mFarClipPlane);
    }

    Mat4<real> Camera::worldToCameraMatrix() const {
        Transform* transform = mGameObject->transform();
        return Mat4<real>::lookAt(transform->localPosition(), transform->localPosition() + transform->forward(), transform->up());
    }

    Mat4<real> Camera::cameraToWorldMatrix() const {
        return Mat4<real>::inverse(worldToCameraMatrix());
    }

    Ray Camera::screenPointToRay(const Vec3& pos) {
        return {mGameObject->transform()->localPosition(), screenToWorldPoint(Vec3::toVec2(pos))};
    }

    Vec3 Camera::screenToWorldPoint(const Vec2& pos) {
        Rect<real> viewport = getViewportRect();
        viewport.x *= Platform::getInstance().getDrawableWidth();
        viewport.y *= Platform::getInstance().getDrawableHeight();
        viewport.w *= Platform::getInstance().getDrawableWidth();
        viewport.h *= Platform::getInstance().getDrawableHeight();

        Vec4 clipCoords;
        clipCoords.x = (2.f * (pos.x - viewport.x)) / viewport.w - 1.f;
        clipCoords.y = (2.f * (viewport.h - pos.y - (1 - viewport.y))) / viewport.h - 1.f;
#ifdef PLATFORM_MACOS
        // metal use 2x2x1 NDC space
        clipCoords.z = 1.f;  // forward
#else
    // Vulkan, D3DX12 coordinate system
#endif
        clipCoords.w = 1.f;

        Vec4 eyeCoords = Mat4<real>::inverse(projectionMatrix()) * clipCoords;
#ifdef PLATFORM_MACOS
        // metal use 2x2x1 NDC space
        eyeCoords.z = 1.f;  // forward
#else
        // Vulkan, D3DX12 coordinate system
        eyeCoords.z = -1.f;  // forward
#endif
        eyeCoords.w = 0.f;

        // NOTE: not cameraToWorldMatrix()
        Vec4 worldCoords = mGameObject->transform()->localToWorldMatrix() * eyeCoords;

        return Vec3{worldCoords.x, worldCoords.y, worldCoords.z}.makeNormalize();
    }

    Vec2 Camera::worldToScreenPoint(const Vec3& pos) {
        // TODO
        return Vec2{};
    }

    bool Camera::isOrthographic() const {
        return mIsOrthographic;
    }

    void Camera::setOrthographic(bool orthographic) {
        mIsOrthographic = orthographic;
    }

    Deg Camera::fieldOfView() const {
        return mFieldOfView;
    }

    real Camera::nearClipPlane() const {
        return mNearClipPlane;
    }

    real Camera::farClipPlane() const {
        return mFarClipPlane;
    }

    BitMask* Camera::cullingMask() {
        return &mCullingMask;
    }

    Rect<real> Camera::getViewportRect() const {
        return mViewportRect;
    }

    void Camera::setViewportRect(Rect<real> viewport) {
        mViewportRect = viewport;
    }

    void Camera::setFieldOfView(Deg fov) {
        mFieldOfView = fov;
    }

    void Camera::setNearClipPlane(real near) {
        mNearClipPlane = near;
    }

    void Camera::setFarClipPlane(real far) {
        mFarClipPlane = far;
    }

    void Camera::setUnitSize(real unitSize) {
        mUnitSize = 1.F / unitSize;
    }

    real Camera::getUnitSize() const {
        return 1.F / mUnitSize;
    }

    void Camera::setTargetTexture(Texture2D* targetTexture) {
        mTargetTexture = targetTexture;
    }

    Texture2D* Camera::getTargetTexture() {
        return mTargetTexture;
    }

    real Camera::aspectRatio() {
        return static_cast<real>(Platform::getInstance().getDrawableWidth()) / static_cast<real>(Platform::getInstance().getDrawableHeight());
    }

    void Camera::update(real deltaTime) {
        // Nothing to do here
    }

    void Camera::render() {
        // Nothing to do here
    }

    Component* Camera::clone() {
        Camera* camera = NEW_T(Camera);
        camera->mIsActive = mIsActive;
        camera->mFieldOfView = mFieldOfView;
        camera->mNearClipPlane = mNearClipPlane;
        camera->mFarClipPlane = mFarClipPlane;
        camera->mIsOrthographic = mIsOrthographic;
        camera->mUnitSize = mUnitSize;
        camera->mViewportRect = mViewportRect;
        camera->mTargetTexture = mTargetTexture;
        camera->mCullingMask = mCullingMask;
        return camera;
    }
}  // namespace GLaDOS