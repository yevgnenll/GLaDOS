#include "Transform.h"
#include "core/GameObject.hpp"
#include "math/Mat4.hpp"
#include "math/Vec3.h"
#include "math/Quat.h"

namespace GLaDOS {
    Transform::Transform() : Component{"Transform"} {
    }

    void Transform::translate(const Vec3& translation, Space relativeTo) {
        if (relativeTo == Space::Self) {
            mLocalPosition += translation;
        } else if (relativeTo == Space::World) {
            mPosition += translation;
        }
        dirty();
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
        dirty();
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
        dirty();
    }

    void Transform::rotateAround(const Vec3& point, const UVec3& axis, Deg angle) {
        // TODO: testme
        Vec3 worldPos = position();
        Quat rotation = Quat::angleAxis(angle, axis);
        Vec3 diff = worldPos - point;
        diff = rotation * diff;
        worldPos = point + diff;
        mPosition = worldPos;
        dirty();
    }

    void Transform::scale(const Vec3& axis, Space relativeTo) {
        if (relativeTo == Space::Self) {
            mLocalScale += axis;
        } else if (relativeTo == Space::World) {
            mLossyScale += axis;
        }
        dirty();
    }

    void Transform::lookAt(const Transform& target, const UVec3& worldUp) {
        // TODO
        dirty();
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
        return Quat::toEuler(mRotation);
    }

    Vec3 Transform::localEulerAngles() const {
        return Quat::toEuler(mLocalRotation);
    }

    Mat4<real> Transform::localToWorldMatrix() const {
        if (mLocalToWorldDirtyFlag) {
            mLocalToWorldMatrixCache = localMatrix() * worldMatrix() * parentLocalMatrix();
            mLocalToWorldDirtyFlag = false;
        }
        return mLocalToWorldMatrixCache;
    }

    Mat4<real> Transform::worldToLocalMatrix() const {
        if (mWorldToLocalDirtyFlag) {
            mWorldToLocalMatrixCache = Mat4<real>::inverse(localToWorldMatrix());
            mWorldToLocalDirtyFlag = false;
        }
        return mWorldToLocalMatrixCache;
    }

    GameObject* Transform::parent() const {
        return mGameObject->mParent;
    }

    void Transform::setPosition(const Vec3& position) {
        mPosition = position;
        dirty();
    }

    void Transform::setLossyScale(const Vec3& scale) {
        mLossyScale = scale;
        dirty();
    }

    void Transform::setEulerAngles(const Vec3& euler) {
        mRotation = Quat::fromEuler(euler).makeNormalize();
        dirty();
    }

    void Transform::setRotation(const Quat& quat) {
        mRotation = quat;
        dirty();
    }

    void Transform::setLocalPosition(const Vec3& position) {
        mLocalPosition = position;
        dirty();
    }

    void Transform::setLocalScale(const Vec3& scale) {
        mLocalScale = scale;
        dirty();
    }

    void Transform::setLocalEulerAngles(const Vec3& euler) {
        mLocalRotation = Quat::fromEuler(euler).makeNormalize();
        dirty();
    }

    void Transform::setLocalRotation(const Quat& quat) {
        mLocalRotation = quat;
        dirty();
    }

    void Transform::setParent(GameObject* parent) {
        mGameObject->mParent->removeChildren(mGameObject);
        mGameObject->mParent = parent;
        mGameObject->mParent->addChildren(mGameObject);
        dirty();
    }

    void Transform::decomposeSRT(const Mat4<real>& transform) {
        Vec3 translation{transform._m44[3][0], transform._m44[3][1], transform._m44[3][2]};
        Quat rotation = Quat::fromRotation(transform);

        Mat4<real> rotScaleMat(
            transform._m44[0][0], transform._m44[0][1], transform._m44[0][2], 0,
            transform._m44[1][0], transform._m44[1][1], transform._m44[1][2], 0,
            transform._m44[2][0], transform._m44[2][1], transform._m44[2][2], 0,
            0, 0, 0, 1
        );
        Mat4<real> invRotMat = Mat4<real>::rotate(Quat::inverse(rotation));
        Mat4<real> scaleSkewMat = rotScaleMat * invRotMat; // cancel rotation

        Vec3 scale = Vec4::toVec3(Mat4<real>::diagonal(scaleSkewMat));

        mLocalPosition = translation;
        mLocalRotation = rotation;
        mLocalScale = scale;
        dirty();
    }

    Vec3 Transform::transformDirection(const UVec3& direction) const {
        Vec3 out;

        // only care vector direction
        out = rotation() * direction;

        return out;
    }

    Vec3 Transform::transformPoint(const Vec3& point) const {
        Vec3 out;

        // first scale (X, Y, Z) each vector components
        Vec3 scaleVector = scale();
        out.x = point.x * scaleVector.x;
        out.y = point.y * scaleVector.y;
        out.z = point.z * scaleVector.z;

        // second rotate vector with quaternion
        out = rotation() * out;

        // third add position to point
        out = position() + out;

        return out;
    }

    Vec3 Transform::transformVector(const Vec3& vector) const {
        Vec3 out;

        // first scale (X, Y, Z) each vector components
        Vec3 scaleVector = scale();
        out.x = vector.x * scaleVector.x;
        out.y = vector.y * scaleVector.y;
        out.z = vector.z * scaleVector.z;

        // second rotate vector with quaternion
        out = rotation() * out;

        return out;
    }

    Vec3 Transform::inverseTransformDirection(const UVec3& direction) const {
    }

    Vec3 Transform::inverseTransformPoint(const Vec3& point) const {
    }

    Vec3 Transform::inverseTransformVector(const Vec3& vector) const {
    }

    Mat4<real> Transform::worldMatrix() const {
        return Mat4<real>::buildSRT(mPosition, mRotation, mLossyScale);
    }

    Mat4<real> Transform::localMatrix() const {
        return Mat4<real>::buildSRT(mLocalPosition, mLocalRotation, mLocalScale);
    }

    Mat4<real> Transform::parentLocalMatrix() const {
        GameObject* parentTransform = parent();
        if ((mGameObject == nullptr) || (parentTransform == nullptr)) {
            return Mat4<real>::identity();
        }

        return parentTransform->mTransform->localMatrix() * parentTransform->mTransform->parentLocalMatrix();
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

    void Transform::dirty() {
        mLocalToWorldDirtyFlag = true;
        mWorldToLocalDirtyFlag = true;
    }
}  // namespace GLaDOS