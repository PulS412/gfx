#include "FileManager.h"

FileManager::FileManager(){
    this->current_directory = GetCurrentWorkingDirectory();
    cout << "current directory: " << current_directory << std::endl;
#ifdef _WIN32
    this->slash = "\\";
#else
    this->slash = "/";
#endif
}

string FileManager::GetCurrentWorkingDirectory() {
#ifdef _WIN32
    char buf[FM_MAX_PATH_LENGTH]; 
    GetCurrentDirectoryA(FM_MAX_PATH_LENGTH, buf);
    return string(buf);
#else
    char buf[FM_MAX_PATH_LENGTH];
    getcwd(buf, sizeof(buf));
    return string(buf);
#endif
}

string FileManager::GetFilePath(const char* _directory, const char* _filename){
    this->file_path = this->current_directory + this->slash + _directory + slash + _filename;
    cout << _filename << " path: " << file_path << endl;
    return file_path;
}