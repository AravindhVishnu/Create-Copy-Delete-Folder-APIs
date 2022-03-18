#ifndef CREATECOPYDELETE_H
#define CREATECOPYDELETE_H

#include <Windows.h>
#include <string>
#include <iostream>
#include <stdlib.h>

class CreateCopyDelete
{
public:
  CreateCopyDelete();
  ~CreateCopyDelete();
  bool createDir(const WCHAR* dir) const;
  bool copyDir(const WCHAR* fromDir, const WCHAR* toDir) const;
  bool deleteDir(const WCHAR* dir) const;
  bool moveDir(const WCHAR* fromDir, const WCHAR* toDir) const;
  bool createFile(const WCHAR* dir, const WCHAR* file) const;
  bool copyFile(const WCHAR* fromDir, const WCHAR* toDir, const WCHAR* file) const;
  bool deleteFile(const WCHAR* dir, const WCHAR* file) const;
  bool moveFile(const WCHAR* fromDir, const WCHAR* toDir, const WCHAR* file) const;
  bool isReadOnly(const WCHAR* path) const;
  void setReadOnly(const WCHAR* path, bool value) const;
  bool getFileSize(const WCHAR* path, uint32_t* fileSize) const;
  bool getDirSize(const WCHAR* dir, uint32_t* dirSize) const;
  uint32_t fileWrite(const WCHAR* path, const WCHAR* str) const;
  uint32_t fileAppendWrite(const WCHAR* path, const WCHAR* str) const;
  uint32_t fileRead(const WCHAR* path, WCHAR* str) const;

private:
  bool directoryExists(const WCHAR* path) const;
  bool fileExists(const WCHAR* path) const;
  bool copyDirectory(std::wstring* fromDir, std::wstring* toDir) const;
  bool deleteDirectory(std::wstring* dir) const;
  bool getDirectorySize(std::wstring* dir, uint32_t* directorySize) const;
};
#endif //CREATECOPYDELETE_H