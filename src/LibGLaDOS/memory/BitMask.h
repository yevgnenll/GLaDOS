#ifndef GLADOS_BITMASK_H
#define GLADOS_BITMASK_H

#include <cstdint>
#include <cstddef>

namespace GLaDOS {
    class BitMask {
      public:
        BitMask() = default;
        ~BitMask() = default;

        void set(uint32_t position);
        void clear(uint32_t position);
        bool isSet(uint32_t position) const;
        void clearAll();
        std::size_t size();

      private:
        uint64_t mArrayOfBits{0x1};
    };
}

#endif  // GLADOS_BITMASK_H
