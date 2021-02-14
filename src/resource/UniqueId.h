#ifndef GLADOS_UNIQUEID_H
#define GLADOS_UNIQUEID_H

#include <atomic>

namespace GLaDOS {
  class UniqueId {
  public:
    UniqueId() : mInstanceId(++typeId) {}

    uint32_t getInstanceId() const { return mInstanceId; }

  private:
    static std::atomic<uint32_t> typeId;

  protected:
    uint32_t mInstanceId;
  };
}  // namespace GLaDOS

#endif  //GLADOS_UNIQUEID_H
