#include "ReferenceCountingMixin.h"
#include "memory/Allocation.h"

namespace GLaDOS {
    ReferenceCountingMixin::ReferenceCountingMixin() : mRefCount{1} {
    }

    ReferenceCountingMixin::ReferenceCountingMixin(const ReferenceCountingMixin& other) : mRefCount{1} {
    }

    ReferenceCountingMixin& ReferenceCountingMixin::operator=(const ReferenceCountingMixin& other) {
        mRefCount = 1;
        return *this;
    }

    void ReferenceCountingMixin::retain() {
        mRefCount++;
    }

    void ReferenceCountingMixin::release() {
        --mRefCount;
        // TODO
//        if (--mRefCount == 0) {
//            // DELETE_T(this, ReferenceCountingMixin) not work here (expression is not assignable, this = nullptr)
//            this->~ReferenceCountingMixin();
//            FREE(this);
//        }
    }
}