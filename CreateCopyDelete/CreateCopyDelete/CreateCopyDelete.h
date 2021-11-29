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
private:
  bool directoryExists(const WCHAR* path);
  bool copyDirectory(std::wstring* fromDir, std::wstring* toDir);
  bool deleteDirectory(std::wstring* dir);
};
#endif //CREATECOPYDELETE_H