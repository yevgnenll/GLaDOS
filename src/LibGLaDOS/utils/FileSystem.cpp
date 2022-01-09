#include "FileSystem.h"

#include <cstdarg>

#include "StringUtils.h"

namespace GLaDOS {
    FileSystem::FileSystem(const std::string& name, OpenMode accessType) {
        open(name, accessType);
    }

    FileSystem::~FileSystem() {
        if (isOpen()) {
            close();
        }
    }

    bool FileSystem::isOpen() {
        return mFilePointer != nullptr;
    }

    bool FileSystem::open(const std::string& name, OpenMode accessType) {
        auto [path, file] = StringUtils::splitFileName(name);
        mPathName = std::move(path);
        mFileName = std::move(file);
        mFullName = name;
        mMode = accessType;

        switch (accessType) {
            case OpenMode::Read:
                mFilePointer = fopen(mFullName.c_str(), "r");
                break;
            case OpenMode::Write:
                mFilePointer = fopen(mFullName.c_str(), "w");
                break;
            case OpenMode::ReadWrite:
                mFilePointer = fopen(mFullName.c_str(), "w+");
                break;
            case OpenMode::AppendOnly:
                mFilePointer = fopen(mFullName.c_str(), "a");
                break;
            case OpenMode::ReadBinary:
                mFilePointer = fopen(mFullName.c_str(), "rb");
                break;
            case OpenMode::WriteBinary:
                mFilePointer = fopen(mFullName.c_str(), "wb");
                break;
            case OpenMode::ReadWriteBinary:
                mFilePointer = fopen(mFullName.c_str(), "wb+");
                break;
            default:
                return false;
        }

        return isOpen();
    }

    void FileSystem::close() {
        fclose(mFilePointer);
        mFilePointer = nullptr;
    }

    void FileSystem::writeFormat(const char* format, ...) {
        va_list args;
        va_start(args, format);
        vfprintf(mFilePointer, format, args);
        va_end(args);
    }

    void FileSystem::readFormat(const char* format, ...) {
        va_list args;
        va_start(args, format);
        vfscanf(mFilePointer, format, args);
        va_end(args);
    }

    bool FileSystem::readAllBytes(std::string& output) {
        if (!isOpen()) {
            return false;
        }

        long fileSize = size();
        output.resize(static_cast<unsigned long>(fileSize));
        std::fread(const_cast<char*>(output.data()), 1, static_cast<std::size_t>(fileSize), mFilePointer);
        return true;
    }

    std::size_t FileSystem::writeBuffer(void* buffer, std::size_t size, std::size_t count) {
        if (!isOpen()) {
            return 0;
        }

        return std::fwrite(buffer, size, count, mFilePointer);
    }

    std::size_t FileSystem::readBuffer(void* buffer, std::size_t size, std::size_t count) {
        if (!isOpen()) {
            return 0;
        }

        return std::fread(buffer, size, count, mFilePointer);
    }

    std::string FileSystem::getLine() {
        return getLine('\n');
    }

    std::string FileSystem::getLine(char delim) {
        std::string temp;

        char c;
        for (;;) {
            c = static_cast<char>(std::fgetc(mFilePointer));
            if (std::feof(mFilePointer) != 0) {
                break;
            }
            if (c == delim) {
                break;
            }
            temp += c;
        }

        return temp;
    }

    std::string FileSystem::getLine(const char* delims) {
        std::string temp;

        char c;
        for (;;) {
            c = static_cast<char>(std::fgetc(mFilePointer));
            if (std::feof(mFilePointer) != 0) {
                break;
            }

            for (const char* temp_delim = delims; temp_delim != nullptr; ++temp_delim) {
                if (c == *temp_delim) {
                    break;
                }
            }

            temp += c;
        }

        return temp;
    }

    long FileSystem::FileSystem::size() {
        seekFromEndOfFile(0);
        long size = std::ftell(mFilePointer);
        rewind();
        return size;
    }

    void FileSystem::rewind() {
        seekFromBeginOfFile(0);
    }

    void FileSystem::seekFromBeginOfFile(int offset) {
        fseek(mFilePointer, offset, SEEK_SET);
    }

    void FileSystem::seekFromEndOfFile(int offset) {
        fseek(mFilePointer, offset, SEEK_END);
    }

    void FileSystem::seekByOffset(int offset) {
        fseek(mFilePointer, offset, SEEK_CUR);
    }

    bool FileSystem::validate() const {
        return (mFilePointer != nullptr) && (feof(mFilePointer) == 0);
    }

    std::string FileSystem::fullName() const {
        return mFullName;
    }

    std::string FileSystem::pathName() const {
        return mPathName;
    }

    std::string FileSystem::fileName() const {
        return mFileName;
    }
}  // namespace GLaDOS