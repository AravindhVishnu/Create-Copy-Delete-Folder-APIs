#ifndef CREATECOPYDELETE_H
#define CREATECOPYDELETE_H

#include <Windows.h>
#include <string>
#include <iostream>

class CreateCopyDelete
{
public:
  CreateCopyDelete();
  ~CreateCopyDelete();
  bool createDir(const WCHAR* dir);
  bool copyDir(const WCHAR* fromDir, const WCHAR* toDir);
  bool deleteDir(const WCHAR* dir);
  bool moveDir(const WCHAR* fromDir, const WCHAR* toDir);
  bool createFile(const WCHAR* dir, const WCHAR* file);
  bool copyFile(const WCHAR* fromDir, const WCHAR* toDir, const WCHAR* file);
  bool deleteFile(const WCHAR* dir, const WCHAR* file);
  bool moveFile(const WCHAR* fromDir, const WCHAR* toDir, const WCHAR* file);
private:
  bool directoryExists(const WCHAR* path);
  bool fileExists(const WCHAR* path);
  bool copyDirectory(std::wstring* fromDir, std::wstring* toDir);
  bool deleteDirectory(std::wstring* dir);
};
#endif //CREATECOPYDELETE_H