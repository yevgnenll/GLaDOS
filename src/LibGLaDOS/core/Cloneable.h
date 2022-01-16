
#ifndef GLADOS_CLONEABLE_H
#define GLADOS_CLONEABLE_H

namespace GLaDOS {
    template <typename T>
    class Cloneable {
      protected:
        virtual T* clone() = 0;
    };
}

#endif  // GLADOS_CLONEABLE_H
