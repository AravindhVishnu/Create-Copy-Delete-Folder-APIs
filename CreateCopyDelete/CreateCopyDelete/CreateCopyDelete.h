#ifndef CREATECOPYDELETE_H
#define CREATECOPYDELETE_H

#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cstddef>
#include <inttypes.h>

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
	void setReadOnly(const WCHAR* path, const bool value) const;
	bool getFileSize(const WCHAR* path, uint32_t* fileSize) const;
	bool getDirSize(const WCHAR* dir, uint32_t* dirSize) const;
	uint32_t fileWrite(const WCHAR* path, const WCHAR* str) const;
	uint32_t fileAppendWrite(const WCHAR* path, const WCHAR* str) const;
	uint32_t fileRead(const WCHAR* path, WCHAR* str) const;
	bool mergeFiles(const WCHAR* pathOne, const WCHAR* pathTwo, const WCHAR* pathMerge, const bool append) const;
	bool findTextInFile(const WCHAR* path, const WCHAR* str, uint32_t* totalFound) const;
	bool replaceTextInFile(const WCHAR* path, const WCHAR* findStr, const WCHAR* replaceStr, uint32_t* totalReplaced) const;
	bool setDirReadOnly(const WCHAR* dir, const bool value) const;
	bool getDirFilePaths(const WCHAR* dir, std::vector<std::wstring>* paths) const;
	bool printDirPaths(const WCHAR* dir, const bool recursive) const;
	bool printFileTime(const WCHAR* path) const;
	bool getDiskCapacityInfo(const WCHAR* path, uint64_t* freeBytesAvailableToCaller, uint64_t* totalNumberOfBytes, uint64_t* totalNumberOfFreeBytes) const;
	uint32_t getDriveType(const WCHAR* path) const;
	bool zipFile(const WCHAR* path, const WCHAR* fileName, const WCHAR* archiveName) const;
	bool unzipFile(const WCHAR* path, const WCHAR* fileName, const WCHAR* archiveName) const;
	bool zipAllFilesInFolder(const WCHAR* dir, const WCHAR* archiveName) const;
	bool unzipAllFilesToFolder(const WCHAR* dir, const WCHAR* archiveName) const;
	bool addFileToZip(const WCHAR* path, const WCHAR* archiveName) const;
	bool removeFileFromZip(const WCHAR* path, const WCHAR* archiveName) const;
	bool printFile(const WCHAR* path) const;
	bool printAllFilesInFolder(const WCHAR* dir) const;

private:
	bool directoryExists(const WCHAR* path) const;
	bool fileExists(const WCHAR* path) const;
	bool copyDirectory(const std::wstring* fromDir, const std::wstring* toDir) const;
	bool deleteDirectory(const std::wstring* dir) const;
	bool getDirectorySize(const std::wstring* dir, uint32_t* directorySize) const;
	bool setDirectoryReadOnly(const std::wstring* dir, const bool value) const;
	bool getDirectoryFilePaths(const std::wstring* dir, std::vector<std::wstring>* paths) const;
	bool printDirectoryPaths(const std::wstring* dir, const bool recursive) const;
	bool splitPath(const std::wstring& path, std::wstring& parent, std::wstring& child) const;
	bool joinPath(std::wstring& path, const std::wstring& parent, const std::wstring& child) const;
	bool printData(std::wstring& str) const;
};
#endif //CREATECOPYDELETE_H