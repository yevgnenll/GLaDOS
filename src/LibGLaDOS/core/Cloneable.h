
#ifndef GLADOS_CLONEABLE_H
#define GLADOS_CLONEABLE_H

namespace GLaDOS {
    template <typename T>
    class Cloneable {
      public:
        virtual ~Cloneable() = default;

      protected:
        virtual T* clone() = 0;
    };
}

#endif  // GLADOS_CLONEABLE_H
