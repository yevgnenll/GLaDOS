#include "BitMask.h"

namespace GLaDOS {
    void BitMask::set(uint32_t position) {
        if (position >= 64) {
            return;
        }
        mArrayOfBits |= ((uint64_t)0x0000000000000001 << position);
    }

    void BitMask::clear(uint32_t position) {
        if (position >= 64) {
            return;
        }
        mArrayOfBits &= ~((uint64_t)0x0000000000000001 << position);
    }

    bool BitMask::isSet(uint32_t position) const {
        if (position >= 64) {
            return false;
        }
        return ((mArrayOfBits >> position) & 0x1) != 0U;
    }

    void BitMask::clearAll() {
        mArrayOfBits = 0x0;
    }

    std::size_t BitMask::size() {
        return sizeof(mArrayOfBits);
    }
}