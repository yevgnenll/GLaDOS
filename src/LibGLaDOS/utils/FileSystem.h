#ifndef GLADOS_FILESYSTEM_H
#define GLADOS_FILESYSTEM_H

#include <string>

#include "Enumeration.h"

namespace GLaDOS {
    class FileSystem {
      public:
        FileSystem(const std::string& name, OpenMode accessType);
        ~FileSystem();

        bool isOpen();
        bool open(const std::string& name, OpenMode accessType);
        void close();  // Close file pointer
        void writeFormat(const char* format, ...);
        void readFormat(const char* format, ...);
        bool readAll(std::string& output);

        template <typename T, std::size_t count>
        std::size_t writeBuffer(const T (&buffer)[count]);
        std::size_t writeBuffer(void* buffer, std::size_t size, std::size_t count);
        template <typename T, std::size_t count>
        std::size_t readBuffer(T (&buffer)[count]);
        std::size_t readBuffer(void* buffer, std::size_t size, std::size_t count);

        std::string getLine();  // Retrieve one line delimited by '\n'
        std::string getLine(char delim);  // Retrieve one line delimited by delim
        std::string getLine(const char* delims);  // Retrieve one line delimited by chars within delims
        long size();

        void rewind();
        void seekFromBeginOfFile(int offset);
        void seekFromEndOfFile(int offset);
        void seekByOffset(int offset);

        bool validate() const;

        std::string fullName() const;
        std::string pathName() const;
        std::string fileName() const;

      private:
        FILE* mFilePointer{nullptr};
        std::string mPathName;
        std::string mFileName;
        std::string mFullName;
        OpenMode mMode{OpenMode::Read};
    };

    template <typename T, std::size_t count>
    std::size_t FileSystem::writeBuffer(const T (&buffer)[count]) {
        if (!isOpen()) {
            return 0;
        }

        return std::fwrite(buffer, sizeof(T), count, mFilePointer);
    }

    template <typename T, std::size_t count>
    std::size_t FileSystem::readBuffer(T (&buffer)[count]) {
        if (!isOpen()) {
            return 0;
        }

        return std::fread(buffer, sizeof(T), count, mFilePointer);
    }
}  // namespace GLaDOS

#endif  //GLADOS_FILESYSTEM_H
