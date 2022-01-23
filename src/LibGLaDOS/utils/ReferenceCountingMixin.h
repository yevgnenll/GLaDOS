#ifndef GLADOS_REFERENCECOUNTINGMIXIN_H
#define GLADOS_REFERENCECOUNTINGMIXIN_H

#include <atomic>

namespace GLaDOS {
    class ReferenceCountingMixin {
      public:
        ReferenceCountingMixin();
        virtual ~ReferenceCountingMixin() = default;
        ReferenceCountingMixin(const ReferenceCountingMixin& other);
        ReferenceCountingMixin& operator=(const ReferenceCountingMixin& other);

        void retain();
        void release();

      protected:
        std::atomic<uint32_t> mRefCount;
    };
}

#endif  // GLADOS_REFERENCECOUNTINGMIXIN_H
