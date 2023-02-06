#include "FileManager.h"

FileManager::FileManager(){
    this->current_directory = GetCurrentWorkingDirectory();
    cout << "current directory: " << current_directory << std::endl;
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
#ifdef _WIN32
    this->file_path = this->current_directory + "\\" + _directory + "\\" + _filename;
#else
    this->file_path = this->current_directory + "/../" + _directory + "/" + _filename;
#endif
    cout << _filename << " path: " << file_path << endl;
    return file_path;
}