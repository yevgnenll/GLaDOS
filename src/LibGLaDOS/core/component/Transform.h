#ifndef GLADOS_TRANSFORM_H
#define GLADOS_TRANSFORM_H

#include "core/Component.h"
#include "math/Quat.h"
#include "math/UVec3.h"
#include "math/Vec3.h"

namespace GLaDOS {
  class Transform : public Component {
  public:
    Transform();
    ~Transform() override = default;

    void translate(const Vec3& translation, Space relativeTo = Space::Self);
    void rotate(const UVec3& axis, Rad angle, Space relativeTo = Space::Self);
    void rotate(const Vec3& eulerAngles, Space relativeTo = Space::Self);
    void rotateAround(const Vec3& point, const UVec3& axis, Deg angle);
    void scale(const Vec3& axis, Space relativeTo = Space::Self);
    void lookAt(const Transform& target, const UVec3& worldUp);

    Vec3 position() const;
    Vec3 scale() const;
    Quat rotation() const;
    Vec3 localPosition() const;
    Vec3 localScale() const;
    Quat localRotation() const;
    UVec3 forward() const;
    UVec3 up() const;
    UVec3 right() const;
    Vec3 eulerAngles() const;
    Vec3 localEulerAngles() const;
    Mat4<real> localToWorldMatrix() const;
    Mat4<real> worldToLocalMatrix() const;
    GameObject* parent() const;

    void setPosition(const Vec3& position);
    void setLossyScale(const Vec3& scale);
    void setEulerAngles(const Vec3& euler);
    void setRotation(const Quat& quat);
    void setLocalPosition(const Vec3& position);
    void setLocalScale(const Vec3& scale);
    void setLocalEulerAngles(const Vec3& euler);
    void setLocalRotation(const Quat& quat);
    void setParent(GameObject* parent);

    // Transforms direction from world space to local space.
    // This operation is not affected by scale or position but only affected by rotate of the transform.
    Vec3 inverseTransformDirection(const Vec3& direction) const;
    // Transforms position from world space to local space.
    // This operation is affected by rotate, scale and position of the transform.
    Vec3 inverseTransformPoint(const Vec3& position) const;
    // Transforms vector from world space to local space.
    // This operation is affected by rotate and scale of the transform.
    Vec3 inverseTransformVector(const Vec3& vector) const;
    // Transforms direction from local space to world space.
    // This operation is not affected by scale or position but only affected by rotate of the
    // transform.
    Vec3 transformDirection(const Vec3& direction) const;
    // Transforms position from local space to world space.
    // This operation is affected by rotate, scale and position of the transform.
    Vec3 transformPoint(const Vec3& position) const;
    // Transforms vector from local space to world space.
    // This operation is affected by rotate and scale of the transform.
    Vec3 transformVector(const Vec3& vector) const;

  protected:
    void update(real deltaTime) override;
    void render() override;

  private:
    Mat4<real> worldMatrix() const;
    Mat4<real> localMatrix() const;
    Mat4<real> parentLocalMatrix() const;

    Vec3 mPosition{Vec3::zero};
    Quat mRotation;
    Vec3 mLossyScale{Vec3::one};
    Vec3 mLocalPosition{Vec3::zero};
    Quat mLocalRotation;
    Vec3 mLocalScale{Vec3::one};
  };
}  // namespace GLaDOS

#endif  //GLADOS_TRANSFORM_H
