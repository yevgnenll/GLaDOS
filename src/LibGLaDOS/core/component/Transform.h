#ifndef GLADOS_TRANSFORM_H
#define GLADOS_TRANSFORM_H

#include "core/Component.h"
#include "math/Quat.h"
#include "math/UVec3.h"
#include "math/Vec3.h"
#include "math/Mat4.hpp"

namespace GLaDOS {
    template <typename T>
    class Mat4;
    class Transform : public Component {
        friend class TransformCurve;
      public:
        Transform();
        ~Transform() override = default;

        void translate(const Vec3& translation, Space relativeTo = Space::Self);
        void rotate(const UVec3& axis, Deg angle, Space relativeTo = Space::Self);
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
        GameObject* parent() const; // can be nullable

        void setPosition(const Vec3& position);
        void setLossyScale(const Vec3& scale);
        void setEulerAngles(const Vec3& euler);
        void setRotation(const Quat& quat);
        void setLocalPosition(const Vec3& position);
        void setLocalScale(const Vec3& scale);
        void setLocalEulerAngles(const Vec3& euler);
        void setLocalRotation(const Quat& quat);
        void setParent(GameObject* parent);
        void decomposeSRT(const Mat4<real>& transform);

        // local space to world space
        Vec3 transformDirection(const UVec3& direction) const; // rotation only
        Vec3 transformPoint(const Vec3& point) const; // position, rotation, and scale
        Vec3 transformVector(const Vec3& vector) const; // rotation and scale only

        // world space to local space
        Vec3 inverseTransformDirection(const UVec3& direction) const;
        Vec3 inverseTransformPoint(const Vec3& point) const;
        Vec3 inverseTransformVector(const Vec3& vector) const;

        Mat4<real> worldMatrix() const;
        Mat4<real> localMatrix() const;
        Mat4<real> parentLocalMatrix() const;

      protected:
        void fixedUpdate(real fixedDeltaTime) override;
        void update(real deltaTime) override;
        void render() override;
        Component* clone() override;
        void dirty();

      private:
        Vec3 mPosition{Vec3::zero};
        Quat mRotation;
        Vec3 mLossyScale{Vec3::one};
        Vec3 mLocalPosition{Vec3::zero};
        Quat mLocalRotation;
        Vec3 mLocalScale{Vec3::one};
        mutable Mat4<real> mLocalToWorldMatrixCache;
        mutable bool mLocalToWorldDirtyFlag{true};
        mutable Mat4<real> mWorldToLocalMatrixCache;
        mutable bool mWorldToLocalDirtyFlag{true};
    };
}  // namespace GLaDOS

#endif  //GLADOS_TRANSFORM_H
