#include "Camera.h"
#include "core/GameObject.hpp"
#include "core/component/Transform.h"
#include "math/Vec2.h"

namespace GLaDOS {
  Camera::Camera() : Component{"Camera"} {
  }

  Camera::Camera(const Vec3& pos) : Component{"Camera"} {
    mGameObject->transform()->setLocalPosition(pos);
  }

  Mat4<real> Camera::projectionMatrix() const {
    if (mIsOrthographic) {
      return Mat4<real>::orthogonal(-10.f, 10.f, -10.f, 10.f, mNearClipPlane, mFarClipPlane);
    }
    return Mat4<real>::perspective(Math::toRadians(fieldOfView()), aspectRatio(), mNearClipPlane, mFarClipPlane);
  }

  Mat4<real> Camera::worldToCameraMatrix() const {
    Transform* transform = mGameObject->transform();
    return Mat4<real>::lookAt(transform->localPosition(), Vec3::normalize(transform->localPosition() + transform->forward()), transform->up());
  }

  Mat4<real> Camera::cameraToWorldMatrix() const {
    return Mat4<real>::inverse(worldToCameraMatrix());
  }

  Ray Camera::screenPointToRay(const Vec3& pos) {
    return {mGameObject->transform()->localPosition(), screenToWorldPoint(Vec2{pos})};
  }

  Vec3 Camera::screenToWorldPoint(const Vec2& pos) {
    Rect<uint32_t> viewport = viewportRect();
    Vec4 clipCoords;
    clipCoords.x = (2.f * (pos.x - viewport.left)) / viewport.right - 1.f;
    clipCoords.y = (2.f * (viewport.bottom - pos.y - (1 - viewport.top))) / viewport.bottom - 1.f;
    clipCoords.z = -1.f;  // forward
    clipCoords.w = 1.f;

    Vec4 eyeCoords = Mat4<real>::inverse(projectionMatrix()) * clipCoords;
    eyeCoords.z = -1.f;  // forward
    eyeCoords.w = 0.f;

    // NOTE: not cameraToWorldMatrix()
    Vec4 worldCoords = mGameObject->transform()->localToWorldMatrix() * eyeCoords;

    return Vec3{worldCoords.x, worldCoords.y, worldCoords.z}.makeNormalize();
  }

  Vec2 Camera::worldToScreenPoint(const Vec3& pos) {
    // TODO
    return Vec2{};
  }

  void Camera::setOrthographic(bool orthographic) {
    mIsOrthographic = orthographic;
  }

  real Camera::fieldOfView() const {
    return mFieldOfView;
  }

  real Camera::nearClipPlane() const {
    return mNearClipPlane;
  }

  real Camera::farClipPlane() const {
    return mFarClipPlane;
  }

  real Camera::aspectRatio() const {
    return static_cast<real>(Platform::getInstance()->width()) / static_cast<real>(Platform::getInstance()->height());
  }

  Rect<uint32_t> Camera::viewportRect() const {
    return mViewportRect; // TODO: Graphics::getViewport()
  }

  void Camera::setFieldOfView(real fov) {
    mFieldOfView = fov;
  }

  void Camera::setNearClipPlane(real near) {
    mNearClipPlane = near;
  }

  void Camera::setFarClipPlane(real far) {
    mFarClipPlane = far;
  }

  void Camera::update(real deltaTime) {
  }

  void Camera::render() {
  }
}  // namespace GLaDOS