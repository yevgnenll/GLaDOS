#include "CircleCollider2D.h"

namespace GLaDOS {
    CircleCollider2D::CircleCollider2D(Vec2 center, real radius) : mCenter{center}, mRadius{radius} {
    }

    CircleCollider2D::~CircleCollider2D() {
    }
}