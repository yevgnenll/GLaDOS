#ifndef GLADOS_UNIQUEID_H
#define GLADOS_UNIQUEID_H

#include <atomic>

namespace GLaDOS {
  class UniqueId {
  public:
    UniqueId() : mInstanceId(++globalIdGenerator) {}

    int getInstanceId() const { return mInstanceId; }

  private:
    static std::atomic_int globalIdGenerator;

  protected:
    int mInstanceId;
  };
}  // namespace GLaDOS

#endif  //GLADOS_UNIQUEID_H
