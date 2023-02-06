#ifndef __FILEMANAGER__
#define __FILEMANAGER__
#include <iostream>
using namespace std;
#ifdef _WIN32
#include <Windows.h>
#define FM_MAX_PATH_LENGTH MAX_PATH
#else
#include <unistd.h>
#include <limits.h>
#define FM_MAX_PATH_LENGTH PATH_MAX
#endif

class FileManager{
private:
    string current_directory;
    string file_path;
    string GetCurrentWorkingDirectory();

public:
    FileManager();
    string GetFilePath(const char* _directory, const char* _filename);
};
#endif