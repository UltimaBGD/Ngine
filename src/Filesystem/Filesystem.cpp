#include "Filesystem.h"

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#define _GNU_SOURCE
#define __USE_GNU
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <linux/limits.h>
#include <dlfcn.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <dlfcn.h>
#include <sys/syslimits.h>
#include <unistd.h>
#include <pwd.h>
#endif

#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include <sstream>

namespace NerdThings::Ngine::Filesystem {
    ////////
    // TPath
    ////////

    // Public Constructors

    TPath::TPath() {
        // Mark as improper
        _HasProperValue = false;
    }

    TPath::TPath(const std::string &pathString_) {
        // Set internal path
        _InternalPath = pathString_;

        // Check path
        __CorrectPath();
    }

    TPath::TPath(const std::string &path_, const std::string &pathB_) {
        // Set internal path
        _InternalPath = path_ + __GetJoinChar() + pathB_;

        // Check path
        __CorrectPath();
    }

    TPath::TPath(const TPath& path_, const TPath& pathB_) {
        // Set internal path
        _InternalPath = path_._InternalPath + __GetJoinChar() + pathB_._InternalPath;

        // Check path
        __CorrectPath();
    }

    TPath::TPath(const TPath &path_, const std::string &pathB_) {
        // Set internal path
        _InternalPath = path_._InternalPath + __GetJoinChar() + pathB_;

        // Check path
        __CorrectPath();
    }

    TPath::TPath(const std::string &path_, const TPath &pathB_) {
        // Set internal path
        _InternalPath = path_ + __GetJoinChar() + pathB_._InternalPath;

        // Check path
        __CorrectPath();
    }

    // Public Methods

    TPath TPath::GetExecutableDirectory() {
#if defined(PLATFORM_UWP)
        auto installed = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
        std::wstring tmp(installed->Begin());
        std::string installedPath(tmp.begin(), tmp.end());

        return installedPath;
#else
        return GetExecutablePath().GetParent();
#endif
    }

    TPath TPath::GetExecutablePath() {
        // https://github.com/cginternals/cpplocate/blob/master/source/liblocate/source/liblocate.c#L39
        // I trust this works as there are no issues about it...
#if defined(_WIN32) && defined(PLATFORM_DESKTOP)
        char exePath[MAX_PATH];
        unsigned int len = GetModuleFileNameA(GetModuleHandleA(nullptr), exePath, MAX_PATH);
        if (len == 0)
            return TPath("");

        return TPath(std::string(exePath));
#elif defined(_WIN32) && defined(PLATFORM_UWP)
        throw std::runtime_error("Cannot get executable path of UWP app, get executable directory instead.");
#elif defined(__linux__)
        char exePath[PATH_MAX];
        int len = readlink("/proc/self/exe", exePath, PATH_MAX);
        if (len <= 0 || len == PATH_MAX)
            return TPath("");

        return TPath(std::string(exePath));
#elif defined(__APPLE__)
        char exePath[PATH_MAX];

        unsigned int len = (unsigned int)PATH_MAX;

        if (_NSGetExecutablePath(exePath, &len) == 0)
        {
            char * realPath = realpath(exePath, 0x0);

            if (realPath == 0x0)
            {
                return TPath("");
            }

            auto pathStr = std::string(realPath);

            free(realPath);

            return TPath(pathStr);
        }
        else
        {
            char * intermediatePath = (char *)malloc(sizeof(char) * len);

            if (_NSGetExecutablePath(intermediatePath, &len) != 0)
            {
                free(intermediatePath);
                return TPath("");
            }

            char * realPath = realpath(intermediatePath, 0x0);

            free(intermediatePath);

            if (realPath == 0x0)
            {
                return TPath("");
            }

            auto pathStr = std::string(realPath);

            free(realPath);

            return TPath(pathStr);
        }

        #else
        // Returns blank, this cannot be used
        return TPath("");
#endif
    }

    std::string TPath::GetFileExtension() const {
        // Get path string
        auto path = GetString();

        // Remove the file extension
        auto dot = path.find_last_of('.');
        if (std::string::npos != dot) {
            return path.substr(dot + 1);
        } else {
            return "";
        }
    }

    std::string TPath::GetObjectName() const {
        // Get path string
        auto nameTemp = GetString();

        // Search for the last directory slash
        auto fSlash = nameTemp.find_last_of(__GetJoinChar());
        if (std::string::npos != fSlash) {
            nameTemp.erase(0, fSlash + 1);
        }

        // Remove the file extension
        auto dot = nameTemp.find_last_of('.');
        if (std::string::npos != dot) {
            nameTemp.erase(dot);
        }

        return nameTemp;
    }

    TPath TPath::GetParent() {
        auto lastSlash = _InternalPath.find_last_of(__GetJoinChar());

        if (std::string::npos != lastSlash)
            return TPath(_InternalPath.substr(0, lastSlash));
        else return TPath();
    }

    std::string TPath::GetString() const {
        return _InternalPath;
    }

    TPath TPath::GetWorkingDirectory() {
#if defined(_WIN32)
        // Create buffer
        DWORD bufferLen = MAX_PATH;
        auto buffer = new char[MAX_PATH + 1];

        // Get current dir
        GetCurrentDirectoryA(MAX_PATH + 1, buffer);

        // Null terminate
        buffer[MAX_PATH] = 0;

        // Convert to string
        std::string string(buffer);

        // Delete buffer
        delete[] buffer;

        // Return
        return TPath(string);
#elif defined(__linux) || defined(__APPLE__)
        // Create buffer
        auto buffer = new char[PATH_MAX];

        // Get working dir
        if (getcwd(buffer, PATH_MAX) == nullptr) throw std::runtime_error("Unable to determine working directory.");

        // Convert to string
        std::string string(buffer);

        // Delete buffer
        delete[] buffer;

        // Return
        return TPath(string);
#endif
    }

    TPath TPath::Join(const std::string &pathA_, const std::string &pathB_) {
        return TPath(pathA_, pathB_);
    }

    TPath TPath::Join(const TPath &pathA_, const TPath &pathB_) {
        return TPath(pathA_, pathB_);
    }

    bool TPath::Valid() const {
        return _HasProperValue;
    }

    // Operators

    TPath operator/(const std::string &path_, const std::string &pathB_) {
        return TPath(path_, pathB_);
    }

    TPath operator/(const TPath &path_, const TPath &pathB_) {
        return TPath(path_, pathB_);
    }

    TPath operator/(const std::string &path_, const TPath &pathB_) {
        return TPath(path_, pathB_);
    }

    TPath operator/(const TPath &path_, const std::string &pathB_) {
        return TPath(path_, pathB_);
    }

    void TPath::operator/=(const TPath &pathB_) {
        // Set internal path
        _InternalPath = _InternalPath + __GetJoinChar() + pathB_._InternalPath;

        // Check path
        __CorrectPath();
    }

    void TPath::operator/=(const std::string &pathB_) {
        // Set internal path
        _InternalPath = _InternalPath + __GetJoinChar() + pathB_;

        // Check path
        __CorrectPath();
    }

    TPath::operator std::string() const {
        return _InternalPath;
    }

    // Private Methods

    std::string TPath::__CleanPathString(const std::string &str_) {
#if defined(_WIN32) && defined(PLATFORM_DESKTOP)
        // Get path length
        auto len = GetShortPathNameA(str_.c_str(), nullptr, 0);

        // Check length
        if (len == 0) {
            auto e = GetLastError();
            if (e == 0x2 || e == 0x3) // File not found/Path not found, cannot clean it
                return str_;
            else throw std::runtime_error("GetShortPathNameA error.");
        }

        // Allocate buffer
        auto buffer = new char[len];

        // Get path
        len = GetShortPathNameA(str_.c_str(), buffer, len);

        // Check length
        if (len == 0) {
            auto e = GetLastError();
            if (e == 0x2 || e == 0x3) // File not found/Path not found, cannot clean it
                return str_;
            else throw std::runtime_error("GetShortPathNameA error.");
        }

        // Convert to string
        auto string = std::string(buffer);

        // Delete buffer
        delete[] buffer;

        return string;
#endif
        return str_;
    }

    void TPath::__CorrectPath() {
        // Clean path
        _InternalPath = __CleanPathString(_InternalPath);

        // Search for empty string
        if (_InternalPath.empty()) {
            // Not a correct value
            _HasProperValue = false;
            return;
        }

        // Look for any non-whitespace chars
        bool hasNonWhitespace = false;
        for (auto c : _InternalPath) {
            if (c != ' ') {
                hasNonWhitespace = true;
                break;
            }
        }

        if (!hasNonWhitespace) {
            // Not a correct value
            _HasProperValue = false;
            return;
        }

        // Look for notation foreign to this OS
        if (__GetJoinChar() != '\\') {
            std::replace(_InternalPath.begin(), _InternalPath.end(), '\\', __GetJoinChar());
        }

        if (__GetJoinChar() != '/') {
            std::replace(_InternalPath.begin(), _InternalPath.end(), '/', __GetJoinChar());
        }

        // Is valid
        _HasProperValue = true;
    }

    char TPath::__GetJoinChar() {
        // TODO: See if there are any more variations
#if defined(_WIN32)
        return '\\';
#else
        return '/';
#endif
    }

    ////////
    // TFilesystemObject
    ////////

    // Public Methods

    void TFilesystemObject::Move(const TPath &newPath_) {
        // Move file
        rename(ObjectPath.GetString().c_str(), newPath_.GetString().c_str());
    }

    void TFilesystemObject::Rename(const std::string &newName_) {
        // Rename
        Move(ObjectPath / newName_);
    }

    std::string TFilesystemObject::GetObjectName() {
        return ObjectPath.GetObjectName();
    }

    TPath TFilesystemObject::GetObjectPath() {
        return ObjectPath;
    }

    // Protected Constructors

    TFilesystemObject::TFilesystemObject(const TPath &path_) : ObjectPath(path_) {}

    ////////
    // TFile
    ////////

    // InternalFileHandler Destructor

    TFile::InternalFileHandler::~InternalFileHandler() {
        if (InternalHandle != nullptr)
            fclose(InternalHandle);
        InternalHandle = nullptr;
    }

    // Public Constructor(s)

    TFile::TFile() : TFilesystemObject(TPath()) {
        // Create an empty handler
        _InternalHandle = std::make_shared<InternalFileHandler>();
    }

    TFile::TFile(const TPath &path_) : TFilesystemObject(path_) {
        // Check path is valid
        if (!path_.Valid()) throw std::runtime_error("File must be given a valid path.");

        // Create an empty handler
        _InternalHandle = std::make_shared<InternalFileHandler>();
    }

    // Destructor

    TFile::~TFile() {
        // Close file in case it isnt gone already
        Close();
    }

    // Public Methods

    void TFile::Close() {
        // Remove handler
        _InternalHandle = nullptr;

        // Set mode
        _InternalOpenMode = MODE_NONE;
    }

    TFile TFile::CreateNewFile(TPath path_, bool leaveOpen_) {
        TFile f(path_);
        f.Open(MODE_WRITE);
        if (!leaveOpen_)
            f.Close();
        return f;
    }

    bool TFile::Delete() {
        // Ensure that we are closed
        Close();

        // Remove from filesystem
        return remove(ObjectPath.GetString().c_str()) == 0;
    }

    bool TFile::Exists() {
        // If we are open, we know we exist
        if (IsOpen()) return true;

        // Using C apis so that it is cross platform
        FILE *file = fopen(ObjectPath.GetString().c_str(), "r");
        if (file != nullptr) {
            fclose(file);
            return true;
        }
        return false;
    }

    EFileOpenMode TFile::GetCurrentMode() {
        return _InternalOpenMode;
    }

    TFile TFile::GetFile(const TPath &path_) {
        return TFile(path_);
    }

    std::string TFile::GetFileExtension() {
        return ObjectPath.GetFileExtension();
    }

    int TFile::GetSize() {
        if (!IsOpen()) {
            ConsoleMessage("Cannot determine size of file that is not open.", "WARN", "TFile");
            return 0;
        }

        fseek(_InternalHandle->InternalHandle, 0, SEEK_END);
        auto s = ftell(_InternalHandle->InternalHandle);
        fseek(_InternalHandle->InternalHandle, 0, SEEK_SET);

        return s;
    }

    bool TFile::IsOpen() {
        if (_InternalHandle == nullptr) return false;

        return _InternalHandle->InternalHandle != nullptr;
    }

    bool TFile::Open(EFileOpenMode mode_, bool binary_) {
        // Check validity of path
        if (!ObjectPath.Valid()) throw std::runtime_error("This file's path is invalid");

        // Open with selected mode
        switch(mode_) {
            case MODE_READ:
                // Open binary file for read
                _InternalHandle->InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "rb" : "r");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            case MODE_WRITE:
                // Open binary file for write
                _InternalHandle->InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "wb" : "w");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            case MODE_APPEND:
                // Open binary file for append
                _InternalHandle->InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "ab" : "a");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            case MODE_READ_WRITE:
                // Open binary file for read and write
                _InternalHandle->InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "w+b" : "w+");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            case MODE_READ_APPEND:
                // Open binary file for read and append
                _InternalHandle->InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "a+b" : "a+");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            default:
                ConsoleMessage("File mode not supported.", "WARN", "TFile");

                // Set mode
                _InternalOpenMode = MODE_NONE;
                break;
        }

        // Return success
        return IsOpen();
    }

    std::vector<unsigned char> TFile::ReadBytes(int size_, int offset_) {
        if (!IsOpen()) throw std::runtime_error("Cannot read from closed file.");

        // Check for our mode
        if (_InternalOpenMode != MODE_READ
            && _InternalOpenMode != MODE_READ_WRITE
            && _InternalOpenMode != MODE_READ_APPEND)
            throw std::runtime_error("File not opened for reading.");

        // Determine file size if size is -1
        auto filesize_ = GetSize();

        if (size_ == -1) {
            size_ = filesize_;
        }

        if (size_ <= 0) {
            throw std::runtime_error("Invalid read size.");
        }

        if (offset_ >= filesize_ || offset_ < 0) {
            throw std::runtime_error("Invalid offset.");
        }

        if (size_ + offset_ > filesize_) {
            throw std::runtime_error("Data out of bounds.");
        }

        // Seek to the offset
        fseek(_InternalHandle->InternalHandle, offset_, SEEK_SET);

        // Read bytes to array
        auto *buffer = new unsigned char[size_];
        fread(buffer, 1, sizeof(buffer), _InternalHandle->InternalHandle);

        // Convert to vector
        std::vector<unsigned char> bytes(buffer, buffer + (sizeof(buffer) / sizeof(buffer[0])));

        // Delete buffer
        delete[] buffer;

        // Return data
        return bytes;
    }

    std::string TFile::ReadString(int size_, int offset_) {
        // Check we're open
        if (!IsOpen()) throw std::runtime_error("Cannot read from closed file.");

        // Check for our mode
        if (_InternalOpenMode != MODE_READ
            && _InternalOpenMode != MODE_READ_WRITE
            && _InternalOpenMode != MODE_READ_APPEND)
            throw std::runtime_error("File not opened for reading.");

        // Determine file size if size is -1
        auto filesize_ = GetSize();

        if (size_ == -1) {
            size_ = filesize_;
        }

        if (size_ <= 0) {
            throw std::runtime_error("Invalid read size.");
        }

        if (offset_ >= filesize_ || offset_ < 0) {
            throw std::runtime_error("Invalid offset.");
        }

        if (size_ + offset_ > filesize_) {
            throw std::runtime_error("Data out of bounds.");
        }

        // Seek to the offset
        fseek(_InternalHandle->InternalHandle, offset_, SEEK_SET);

        // Read to c string
        auto buffer = new char[size_ + 1];
        int r = fread(buffer, 1, size_, _InternalHandle->InternalHandle);

        // Null-terminate buffer
        buffer[r] = '\0';

        // Convert to string
        auto str = std::string(buffer);

        // Delete buffer
        delete[] buffer;

        return str;
    }

    bool TFile::WriteBytes(std::vector<unsigned char> data_) {
        // Check we're open
        if (!IsOpen()) throw std::runtime_error("Cannot write to a closed file.");

        // Check for our mode
        if (_InternalOpenMode != MODE_WRITE
            && _InternalOpenMode != MODE_APPEND
            && _InternalOpenMode != MODE_READ_WRITE
            && _InternalOpenMode != MODE_READ_APPEND)
            throw std::runtime_error("File not opened for writing.");

        // Calculate size
        auto dataSize = data_.size() * sizeof(unsigned char);

        // Write
        return fwrite(data_.data(), 1, dataSize, _InternalHandle->InternalHandle) == 1;
    }

    bool TFile::WriteString(const std::string &string_) {
        // Check we're open
        if (!IsOpen()) throw std::runtime_error("Cannot write to closed file.");

        // Check for our mode
        if (_InternalOpenMode != MODE_WRITE
            && _InternalOpenMode != MODE_APPEND
            && _InternalOpenMode != MODE_READ_WRITE
            && _InternalOpenMode != MODE_READ_APPEND)
            throw std::runtime_error("File not opened for writing.");

        // Write string
        return fputs(string_.c_str(), _InternalHandle->InternalHandle) != EOF;
    }

    ////////
    // TDirectory
    ////////

    TDirectory::TDirectory() : TFilesystemObject(TPath()) {}

    TDirectory::TDirectory(const TPath &path_) : TFilesystemObject(path_) {
        // Check for valid path
        if (!path_.Valid()) throw std::runtime_error("Directory must be given a valid path.");

        // Check it exists
        if (!Exists()) throw std::runtime_error("Directory does not exist.");
    }

    std::pair<bool, TDirectory> TDirectory::Create(const TPath &path_) {
        auto success = false;
#if defined(_WIN32)
        // Create directory
        success = CreateDirectoryA(path_.GetString().c_str(), nullptr) != 0;
#elif defined(__linux__) || defined(__APPLE__)
        // Create directory
        success = mkdir(path_.GetString().c_str(), 0777) == 0;
#endif
        if (success)
            return {success, TDirectory(path_)};
        return {success, TDirectory()};
    }

    bool TDirectory::Delete() {
#if defined(_WIN32)
        // Try to delete (not recursive)
        auto del = RemoveDirectoryA(ObjectPath.GetString().c_str());
        return del != 0;
#elif defined(__linux__) || defined(__APPLE__)
        return remove(ObjectPath.GetString().c_str()) == 0;
#endif
        return false;
    }

    bool TDirectory::DeleteRecursive() {
        // Success tracker
        auto success = true;

        // Delete my own files
        for (auto file : GetFiles()) {
            if (!file.Delete()) {
                success = false;
                break;
            }
        }

        // Stop if we find an issue
        if (!success)
            return false;

        // Get directories
        auto dirs = GetDirectories();

        // Delete child directories
        for (auto dir : dirs) {
            if (!dir.DeleteRecursive()) {
                success = false;
                break;
            }
        }

        // Stop if we find an issue
        if (!success)
            return false;


        // Delete self
        success = Delete();

        // Return
        return success;
    }

    bool TDirectory::Exists() {
#if defined(_WIN32)
        // https://stackoverflow.com/a/6218445
        // Get attributes for directory
        DWORD dwAttrib = GetFileAttributesA(ObjectPath.GetString().c_str());

        return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
                (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#elif defined(__linux__) || defined(__APPLE__)
        // Test opening of file
        DIR *dir = opendir(ObjectPath.GetString().c_str());
        if (dir) {
            closedir(dir);
            return true;
        }
        return false;
#endif
        return false;
    }

    std::vector<TDirectory> TDirectory::GetDirectories() {
        // Check exists
        if (!Exists()) throw std::runtime_error("This directory does not exist.");

        // Directories vector
        auto dirs = std::vector<TDirectory>();
#if defined(__linux__) || defined(__APPLE__)
        // Variables
        DIR *dir;
        dirent *entry;

        // Open dir
        dir = opendir(ObjectPath.GetString().c_str());

        // Test open
        if (!dir) throw std::runtime_error("Cannot open directory.");

        // Read all directories
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_DIR) {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                    dirs.push_back(TDirectory(TPath(ObjectPath, entry->d_name)));
            }
        }

        // Close directory
        closedir(dir);
#elif defined(_WIN32) && defined(PLATFORM_DESKTOP)
        // Find first directory
        WIN32_FIND_DATA FindFileData;
        HANDLE hFind = FindFirstFileA((ObjectPath.GetString() + "\\*").c_str(), &FindFileData);

        // Check exists
        if (hFind == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Invalid directory.");
        }

        // Search for directories
        do {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                auto dirName = FindFileData.cFileName;
                // Avoids . and .. directories
                if (strcmp(dirName, ".") != 0 && strcmp(dirName, "..") != 0)
                    dirs.push_back(TDirectory(TPath(ObjectPath, dirName)));
            }
        } while (FindNextFile(hFind, &FindFileData) != 0);

        // Close search
        FindClose(hFind);
#endif
        return dirs;
    }

    std::vector<TFile> TDirectory::GetFiles() {
        // Check exists
        if (!Exists()) throw std::runtime_error("This directory does not exist.");

        // Files vector
        auto files = std::vector<TFile>();
#if defined(__linux__) || defined(__APPLE__)
        // Variables
        DIR *dir;
        dirent *entry;

        // Open dir
        dir = opendir(ObjectPath.GetString().c_str());

        // Test open
        if (!dir) throw std::runtime_error("Cannot open directory.");

        // Read all directories
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type != DT_DIR) {
                files.push_back(TFile(TPath(ObjectPath, entry->d_name)));
            }
        }

        // Close directory
        closedir(dir);
#elif defined(_WIN32)
        // Find the first file in the directory
        WIN32_FIND_DATAA FindFileData;
        HANDLE hFind = FindFirstFileA((ObjectPath.GetString() + "\\*").c_str(), &FindFileData);

        // Check it exists
        if (hFind == INVALID_HANDLE_VALUE) {
            auto err = GetLastError();
            throw std::runtime_error("Invalid directory.");
        }

        // Get all files
        do {
            if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                auto filename = FindFileData.cFileName;
                files.push_back(TFile(TPath(ObjectPath, filename)));
            }
        } while (FindNextFileA(hFind, &FindFileData) != 0);

        // Close search
        FindClose(hFind);
#endif
        return files;
    }

    std::vector<TFile> TDirectory::GetFilesRecursive() {
        // Keep track of all files
        auto files = std::vector<TFile>();

        // Grab my files
        auto myFiles = GetFiles();
        files.insert(files.end(), myFiles.begin(), myFiles.end());

        // Get all descendant directories
        auto dirs = GetDirectories();

        for (auto dir : dirs) {
            auto dirFiles = dir.GetFilesRecursive();
            files.insert(files.end(), dirFiles.begin(), dirFiles.end());
        }

        return files;
    }

    TDirectory TDirectory::GetDirectory(const TPath &path_) {
        return TDirectory(path_);
    }

    TDirectory TDirectory::GetExecutableDirectory() {
        return TDirectory(TPath::GetExecutableDirectory());
    }

    TDirectory TDirectory::GetWorkingDirectory() {
        return TDirectory(TPath::GetWorkingDirectory());
    }
}