#include "Transform.h"

#include "core/GameObject.hpp"
#include "math/Mat4.hpp"

namespace GLaDOS {
    Transform::Transform() : Component{"Transform"} {
    }

    void Transform::translate(const Vec3& translation, Space relativeTo) {
        if (relativeTo == Space::Self) {
            mLocalPosition += translation;
        } else if (relativeTo == Space::World) {
            mPosition += translation;
        }
    }

    void Transform::rotate(const UVec3& axis, Deg angle, Space relativeTo) {
        Quat angleAxis = Quat::angleAxis(angle, axis);
        if (relativeTo == Space::Self) {
            mLocalRotation = angleAxis * mLocalRotation;
            mLocalRotation.makeNormalize();
        } else if (relativeTo == Space::World) {
            mRotation = angleAxis * mRotation;
            mRotation.makeNormalize();
        }
    }

    void Transform::rotate(const Vec3& eulerAngles, Space relativeTo) {
        Quat euler = Quat::fromEuler(eulerAngles);
        if (relativeTo == Space::Self) {
            mLocalRotation = euler * mLocalRotation;
            mLocalRotation.makeNormalize();
        } else if (relativeTo == Space::World) {
            mRotation = euler * mRotation;
            mRotation.makeNormalize();
        }
    }

    void Transform::rotateAround(const Vec3& point, const UVec3& axis, Deg angle) {
        // TODO
    }

    void Transform::scale(const Vec3& axis, Space relativeTo) {
        if (relativeTo == Space::Self) {
            mLocalScale += axis;
        } else if (relativeTo == Space::World) {
            mLossyScale += axis;
        }
    }

    void Transform::lookAt(const Transform& target, const UVec3& worldUp) {
        // TODO
    }

    Vec3 Transform::position() const {
        return mLocalPosition + mPosition;
    }

    Vec3 Transform::scale() const {
        return mLocalScale + mLossyScale;
    }

    Quat Transform::rotation() const {
        return mLocalRotation * mRotation;
    }

    Vec3 Transform::localPosition() const {
        return mLocalPosition;
    }

    Vec3 Transform::localScale() const {
        return mLocalScale;
    }

    Quat Transform::localRotation() const {
        return mLocalRotation;
    }

    UVec3 Transform::forward() const {
        return (rotation() * Vec3::forward).makeNormalize();
    }

    UVec3 Transform::up() const {
        return (rotation() * Vec3::up).makeNormalize();
    }

    UVec3 Transform::right() const {
        return (Vec3::cross(forward(), up())).makeNormalize();
    }

    Vec3 Transform::eulerAngles() const {
        return localEulerAngles() * Quat::toEuler(mRotation);
    }

    Vec3 Transform::localEulerAngles() const {
        return Quat::toEuler(mLocalRotation);
    }

    Mat4<real> Transform::localToWorldMatrix() const {
        return localMatrix() * worldMatrix() * parentLocalMatrix();
    }

    Mat4<real> Transform::worldToLocalMatrix() const {
        return Mat4<real>::inverse(localToWorldMatrix());
    }

    GameObject* Transform::parent() const {
        return mGameObject->mParent;
    }

    void Transform::setPosition(const Vec3& position) {
        mPosition = position;
    }

    void Transform::setLossyScale(const Vec3& scale) {
        mLossyScale = scale;
    }

    void Transform::setEulerAngles(const Vec3& euler) {
        mRotation = Quat::fromEuler(euler).makeNormalize();
    }

    void Transform::setRotation(const Quat& quat) {
        mRotation = quat;
    }

    void Transform::setLocalPosition(const Vec3& position) {
        mLocalPosition = position;
    }

    void Transform::setLocalScale(const Vec3& scale) {
        mLocalScale = scale;
    }

    void Transform::setLocalEulerAngles(const Vec3& euler) {
        mLocalRotation = Quat::fromEuler(euler).makeNormalize();
    }

    void Transform::setLocalRotation(const Quat& quat) {
        mLocalRotation = quat;
    }

    void Transform::setParent(GameObject* parent) {
        mGameObject->mParent = parent;
        parent->mChildren.emplace_back(mGameObject);
    }

    Vec3 Transform::inverseTransformDirection(const Vec3& direction) const {
        return Quat::fromEuler(Mat4<real>::decomposeRotation(worldToLocalMatrix())) * direction;
    }

    Vec3 Transform::inverseTransformPoint(const Vec3& position) const {
        Mat4<real> localMat = worldToLocalMatrix();
        return Quat::fromEuler(Mat4<real>::decomposeRotation(localMat)) * (position * Mat4<real>::decomposeScale(localMat)) + Mat4<real>::decomposeTranslation(localMat);
    }

    Vec3 Transform::inverseTransformVector(const Vec3& vector) const {
        Mat4<real> localMat = worldToLocalMatrix();
        return Quat::fromEuler(Mat4<real>::decomposeRotation(localMat)) * (vector * Mat4<real>::decomposeScale(localMat));
    }

    Vec3 Transform::transformDirection(const Vec3& direction) const {
        Mat4<real> localMat = localToWorldMatrix();
        return Quat::fromEuler(Mat4<real>::decomposeRotation(localMat)) * direction;
    }

    Vec3 Transform::transformPoint(const Vec3& position) const {
        Mat4<real> localMat = localToWorldMatrix();
        return Quat::fromEuler(Mat4<real>::decomposeRotation(localMat)) * (position * Mat4<real>::decomposeScale(localMat)) + Mat4<real>::decomposeTranslation(localMat);
    }

    Vec3 Transform::transformVector(const Vec3& vector) const {
        Mat4<real> localMat = localToWorldMatrix();
        return Quat::fromEuler(Mat4<real>::decomposeRotation(localMat)) * (vector * Mat4<real>::decomposeScale(localMat));
    }

    Mat4<real> Transform::worldMatrix() const {
        return Mat4<real>::scale(mLossyScale) * Mat4<real>::rotate(mRotation) * Mat4<real>::translate(mPosition);
    }

    Mat4<real> Transform::localMatrix() const {
        return Mat4<real>::scale(mLocalScale) * Mat4<real>::rotate(mLocalRotation) * Mat4<real>::translate(mLocalPosition);
    }

    Mat4<real> Transform::parentLocalMatrix() const {
        if ((mGameObject == nullptr) || (mGameObject->mParent == nullptr)) {
            return Mat4<real>::identity();
        }

        return mGameObject->mParent->mTransform->localMatrix() * mGameObject->mParent->mTransform->parentLocalMatrix();
    }

    void Transform::fixedUpdate(real fixedDeltaTime) {
        // Nothing to do here
    }

    void Transform::update(real deltaTime) {
        // Nothing to do here
    }

    void Transform::render() {
        // Nothing to do here
    }

    Component* Transform::clone() {
        Transform* transform = NEW_T(Transform);
        transform->mIsActive = mIsActive;
        transform->mPosition = mPosition;
        transform->mRotation = mRotation;
        transform->mLocalScale = mLossyScale;
        transform->mLocalPosition = mLocalPosition;
        transform->mLocalRotation = mLocalRotation;
        transform->mLocalScale = mLocalScale;
        return transform;
    }
}  // namespace GLaDOS