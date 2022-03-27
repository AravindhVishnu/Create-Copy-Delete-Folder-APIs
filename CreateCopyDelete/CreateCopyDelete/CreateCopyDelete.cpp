#include "CreateCopyDelete.h"

CreateCopyDelete::CreateCopyDelete()
{
}

CreateCopyDelete::~CreateCopyDelete()
{
}

bool CreateCopyDelete::fileExists(const WCHAR* path) const
{
	DWORD attributes = GetFileAttributes(path);
	return (attributes != INVALID_FILE_ATTRIBUTES &&
		!(attributes & FILE_ATTRIBUTE_DIRECTORY));
}

bool CreateCopyDelete::directoryExists(const WCHAR* path) const
{
	DWORD attributes = ::GetFileAttributes(path);
	if (attributes == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}
	if (attributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;
	}
	return false;
}

bool CreateCopyDelete::createDir(const WCHAR* dir) const
{
	std::wstring newDir = dir;

	if (this->directoryExists(dir) == true)
	{
		::printf("Error: Directory '%S' already exists\n", newDir.c_str());
		return false;
	}
	else
	{
		::printf("Creating directory '%S'\n", newDir.c_str());

		if (::CreateDirectory(newDir.c_str(), NULL) == 0)
		{
			::printf("Error: Failed to create: %S\n", newDir.c_str());
			return false;
		}

		return true;
	}
}

bool CreateCopyDelete::copyDir(const WCHAR* fromDir, const WCHAR* toDir) const
{
	if (this->directoryExists(fromDir) == false)
	{
		::printf("Error: This directory does not exist: %S\n", fromDir);
		return false;
	}
	if (this->directoryExists(toDir) == true)
	{
		::printf("Error: This directory already exists: %S\n", toDir);
		return false;
	}

	std::wstring inDir = fromDir;
	std::wstring outDir = toDir;
	::printf("Copying directory from '%S' to '%S'\n", inDir.c_str(), outDir.c_str());
	return this->copyDirectory(&inDir, &outDir);
}

bool CreateCopyDelete::deleteDir(const WCHAR* dir) const
{
	if (this->directoryExists(dir) == true)
	{
		std::wstring inDir = dir;
		::printf("Deleting directory '%S'\n", inDir.c_str());
		return this->deleteDirectory(&inDir);
	}
	else
	{
		::printf("Error: This directory does not exist: %S\n", dir);
		return false;
	}
}

bool CreateCopyDelete::moveDir(const WCHAR* fromDir, const WCHAR* toDir) const
{
	return this->copyDir(fromDir, toDir) && this->deleteDir(fromDir);
}

bool CreateCopyDelete::getDirSize(const WCHAR* dir, uint32_t* dirSize) const
{
	if (this->directoryExists(dir) == true)
	{
		std::wstring inDir = dir;
		if (this->getDirectorySize(&inDir, dirSize) == true)
		{
			return true;
		}
		else
		{
			::printf("Error: Failed to get the directory size");
			return false;
		}
	}
	else
	{
		::printf("Error: This directory does not exist: %S\n", dir);
		return false;
	}
}

bool CreateCopyDelete::copyDirectory(const std::wstring* fromDir, const std::wstring* toDir) const
{
	HANDLE hSearch;
	WIN32_FIND_DATA itemData;
	std::wstring searchFilter;
	std::wstring fromSubDir;
	std::wstring toSubDir;

	std::wstring copyFrom;
	std::wstring copyTo;

	::CreateDirectory(toDir->c_str(), NULL);

	searchFilter = *fromDir;
	searchFilter += L"\\*";

	hSearch = ::FindFirstFile(searchFilter.c_str(), &itemData);
	while (hSearch != INVALID_HANDLE_VALUE)
	{
		if (wcscmp(itemData.cFileName, L".") != 0 && wcscmp(itemData.cFileName, L"..") != 0)
		{
			if ((itemData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			{
				fromSubDir.clear();
				fromSubDir = *fromDir;
				fromSubDir += L"\\";
				fromSubDir += itemData.cFileName;

				toSubDir.clear();
				toSubDir = *toDir;
				toSubDir += L"\\";
				toSubDir += itemData.cFileName;

				this->copyDirectory(&fromSubDir, &toSubDir);
			}
			else
			{
				copyFrom.clear();
				copyFrom = *fromDir;
				copyFrom += L"\\";
				copyFrom += itemData.cFileName;

				copyTo.clear();
				copyTo = *toDir;
				copyTo += L"\\";
				copyTo += itemData.cFileName;

				::CopyFile(copyFrom.c_str(), copyTo.c_str(), FALSE);
			}
		}

		if (::FindNextFile(hSearch, &itemData) == 0)
		{
			::FindClose(hSearch);
			hSearch = INVALID_HANDLE_VALUE;
		}
	}

	return true;
}

bool CreateCopyDelete::deleteDirectory(const std::wstring* dir) const
{
	HANDLE hSearch;
	WIN32_FIND_DATA itemData;
	std::wstring searchFilter;
	std::wstring subDir;
	std::wstring file;

	searchFilter = *dir;
	searchFilter += L"\\*";

	hSearch = ::FindFirstFile(searchFilter.c_str(), &itemData);
	while (hSearch != INVALID_HANDLE_VALUE)
	{
		if (wcscmp(itemData.cFileName, L".") != 0 && wcscmp(itemData.cFileName, L"..") != 0)
		{
			if ((itemData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			{
				subDir.clear();
				subDir = *dir;
				subDir += L"\\";
				subDir += itemData.cFileName;

				this->deleteDirectory(&subDir);
			}
			else
			{
				file.clear();
				file = *dir;
				file += L"\\";
				file += itemData.cFileName;

				::SetFileAttributes(file.c_str(), FILE_ATTRIBUTE_NORMAL);
				::DeleteFile(file.c_str());
			}
		}

		if (::FindNextFile(hSearch, &itemData) == 0)
		{
			::FindClose(hSearch);
			hSearch = INVALID_HANDLE_VALUE;
		}
	}

	::RemoveDirectory(dir->c_str());

	return true;
}

bool CreateCopyDelete::createFile(const WCHAR* dir, const WCHAR* file) const
{
	if (this->directoryExists(dir) == true)
	{
		std::wstring directory = dir;
		std::wstring fileCreate = file;
		std::wstring doubleBackslash(L"\\");

		std::wstring path = directory.append(doubleBackslash);
		path = path.append(file);

		HANDLE handle = ::CreateFile(
			path.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (handle == INVALID_HANDLE_VALUE)
		{
			DWORD err = ::GetLastError();
			::printf("Error: %d\n", err);
			return false;
		}
		else
		{
			::CloseHandle(handle);
			::printf("Creating file '%S'\n", path.c_str());
			return true;
		}
	}
	else
	{
		::printf("Error: This directory does not exist: %S\n", dir);
		return false;
	}
}

bool CreateCopyDelete::copyFile(const WCHAR* fromDir, const WCHAR* toDir, const WCHAR* file) const
{
	if (this->directoryExists(fromDir) == false)
	{
		::printf("Error: This directory does not exist: %S\n", fromDir);
		return false;
	}
	if (this->directoryExists(toDir) == false)
	{
		::printf("Error: This directory does not exist: %S\n", toDir);
		return false;
	}

	std::wstring fromDirectory = fromDir;
	std::wstring fileCopy = file;
	std::wstring doubleBackslash(L"\\");

	std::wstring fromPath = fromDirectory.append(doubleBackslash);
	fromPath = fromPath.append(file);

	if (this->fileExists(fromPath.c_str()) == false)
	{
		::printf("Error: This file does not exist: %S\n", fromPath.c_str());
		return false;
	}

	std::wstring toDirectory = toDir;
	std::wstring toPath = toDirectory.append(doubleBackslash);
	toPath = toPath.append(file);

	if (this->fileExists(toPath.c_str()) == true)
	{
		::printf("Error: This file already exists: %S\n", toPath.c_str());
		return false;
	}

	if (::CopyFile(fromPath.c_str(), toPath.c_str(), FALSE) == 0)
	{
		DWORD err = ::GetLastError();
		::printf("Error: %d\n", err);
		return false;
	}
	else
	{
		::printf("Copying file from '%S' to '%S'\n", fromPath.c_str(), toPath.c_str());
		return true;
	}
}

bool CreateCopyDelete::deleteFile(const WCHAR* dir, const WCHAR* file) const
{
	if (this->directoryExists(dir) == true)
	{
		std::wstring directory = dir;
		std::wstring fileDelete = file;
		std::wstring doubleBackslash(L"\\");

		std::wstring path = directory.append(doubleBackslash);
		path = path.append(file);

		if (this->fileExists(path.c_str()) == true)
		{
			if (::DeleteFile(path.c_str()) == 0)
			{
				DWORD err = ::GetLastError();
				::printf("Error: %d\n", err);
				return false;
			}
			else
			{
				::printf("Deleting file '%S'\n", path.c_str());
				return true;
			}
		}
		else
		{
			::printf("Error: This file does not exist: %S\n", path.c_str());
			return false;
		}
	}
	else
	{
		::printf("Error: This directory does not exist: %S\n", dir);
		return false;
	}
}

bool CreateCopyDelete::moveFile(const WCHAR* fromDir, const WCHAR* toDir, const WCHAR* file) const
{
	return this->copyFile(fromDir, toDir, file) && this->deleteFile(fromDir, file);
}

bool CreateCopyDelete::isReadOnly(const WCHAR* path) const
{
	DWORD attributes = ::GetFileAttributes(path);
	if (attributes != INVALID_FILE_ATTRIBUTES)
	{
		if ((attributes & FILE_ATTRIBUTE_READONLY) != 0)
		{
			return true;
		}
	}
	return false;
}

void CreateCopyDelete::setReadOnly(const WCHAR* path, const bool value) const
{
	DWORD attributes = ::GetFileAttributes(path);
	if (attributes != INVALID_FILE_ATTRIBUTES)
	{
		if (value == true)
		{
			attributes = attributes | FILE_ATTRIBUTE_READONLY;
		}
		else
		{
			attributes = attributes & ~FILE_ATTRIBUTE_READONLY;
		}
		::SetFileAttributes(path, attributes);
	}
}

bool CreateCopyDelete::getFileSize(const WCHAR* path, uint32_t* fileSize) const
{
	*fileSize = 0;
	if (this->fileExists(path) == true)
	{
		HANDLE handle = ::CreateFile(
			path,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (handle == INVALID_HANDLE_VALUE)
		{
			DWORD err = ::GetLastError();
			::printf("Error: %d\n", err);
			return false;
		}
		else
		{
			LARGE_INTEGER size;
			bool result = ::GetFileSizeEx(handle, &size);
			if (result != 0)
			{
				*fileSize = static_cast<uint32_t>(size.QuadPart);
				::CloseHandle(handle);
				return true;
			}
			else
			{
				DWORD err = ::GetLastError();
				::printf("Error: %d\n", err);
				::CloseHandle(handle);
				return false;
			}
		}
	}
	else
	{
		::printf("Error: This file does not exist: %S\n", path);
		return false;
	}
}

bool CreateCopyDelete::getDirectorySize(const std::wstring* dir, uint32_t* directorySize) const
{
	HANDLE hSearch;
	WIN32_FIND_DATA itemData;
	std::wstring searchFilter;
	std::wstring subDir;
	std::wstring file;

	searchFilter = *dir;
	searchFilter += L"\\*";

	hSearch = ::FindFirstFile(searchFilter.c_str(), &itemData);
	while (hSearch != INVALID_HANDLE_VALUE)
	{
		if (wcscmp(itemData.cFileName, L".") != 0 && wcscmp(itemData.cFileName, L"..") != 0)
		{
			if ((itemData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			{
				subDir.clear();
				subDir = *dir;
				subDir += L"\\";
				subDir += itemData.cFileName;

				this->getDirectorySize(&subDir, directorySize);
			}
			else
			{
				file.clear();
				file = *dir;
				file += L"\\";
				file += itemData.cFileName;

				uint32_t fileSize = 0;
				this->getFileSize(file.c_str(), &fileSize);
				*directorySize += fileSize;
			}
		}

		if (::FindNextFile(hSearch, &itemData) == 0)
		{
			::FindClose(hSearch);
			hSearch = INVALID_HANDLE_VALUE;
		}
	}

	return true;
}

uint32_t CreateCopyDelete::fileWrite(const WCHAR* path, const WCHAR* str) const
{
	DWORD bytesWritten = 0;
	if (this->fileExists(path) == true)
	{
		HANDLE handle = ::CreateFile(
			path,
			GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (handle == INVALID_HANDLE_VALUE)
		{
			DWORD err = ::GetLastError();
			::printf("Error: %d\n", err);
			return 0;
		}
		else
		{
			if (::WriteFile(handle, str, static_cast<DWORD>(wcslen(str) * sizeof(wchar_t)), &bytesWritten, NULL) == 0)
			{
				DWORD err = ::GetLastError();
				::printf("Error: %d\n", err);
				return 0;
			}
			if (::FlushFileBuffers(handle) == 0)
			{
				DWORD err = ::GetLastError();
				::printf("Error: %d\n", err);
				return 0;
			}
			::CloseHandle(handle);
		}
	}
	return bytesWritten;
}

uint32_t CreateCopyDelete::fileAppendWrite(const WCHAR* path, const WCHAR* str) const
{
	DWORD bytesWritten = 0;
	if (this->fileExists(path) == true)
	{
		HANDLE handle = ::CreateFile(
			path,
			FILE_APPEND_DATA,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (handle == INVALID_HANDLE_VALUE)
		{
			DWORD err = ::GetLastError();
			::printf("Error: %d\n", err);
			return 0;
		}
		else
		{
			if (::SetFilePointer(handle, 0, NULL, FILE_END) == INVALID_SET_FILE_POINTER)
			{
				::printf("Error: Unable to set file pointer to end of file.\n");
				return 0;
			}
			if (::WriteFile(handle, str, static_cast<DWORD>(wcslen(str) * sizeof(wchar_t)), &bytesWritten, NULL) == 0)
			{
				DWORD err = ::GetLastError();
				::printf("Error: %d\n", err);
				return 0;
			}
			if (::FlushFileBuffers(handle) == 0)
			{
				DWORD err = ::GetLastError();
				::printf("Error: %d\n", err);
				return 0;
			}
			::CloseHandle(handle);
		}
	}
	return bytesWritten;
}

uint32_t CreateCopyDelete::fileRead(const WCHAR* path, WCHAR* str) const
{
	DWORD bytesRead = 0;
	if (this->fileExists(path) == true)
	{
		uint32_t size = 0;
		this->getFileSize(path, &size);

		HANDLE handle = ::CreateFile(
			path,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (handle == INVALID_HANDLE_VALUE)
		{
			DWORD err = ::GetLastError();
			::printf("Error: %d\n", err);
			return 0;
		}
		else
		{
			if (::ReadFile(handle, str, static_cast<DWORD>(size), &bytesRead, NULL) == 0)
			{
				DWORD err = ::GetLastError();
				::printf("Error: %d\n", err);
				return 0;
			}
			::CloseHandle(handle);
		}
	}
	return bytesRead;
}

bool CreateCopyDelete::mergeFiles(const WCHAR* pathOne, const WCHAR* pathTwo, const WCHAR* pathMergeFolder, const WCHAR* pathMergeFile, const bool append) const
{
	if (this->fileExists(pathOne) == false)
	{
		::printf("Error: This file does not exist: %S\n", pathOne);
		return false;
	}
	if (this->fileExists(pathTwo) == false)
	{
		::printf("Error: This file does not exist: %S\n", pathTwo);
		return false;
	}

	std::wstring directory = pathMergeFolder;
	std::wstring file = pathMergeFile;
	std::wstring doubleBackslash(L"\\");
	std::wstring path = directory.append(doubleBackslash);
	path = path.append(file);
	const WCHAR* pathMerge = path.c_str();

	if (append == false)
	{
		if (this->fileExists(pathMerge) == true)
		{
			::printf("Error: This file already exists: %S\n", pathMerge);
			return false;
		}
		if (this->createFile(pathMergeFolder, pathMergeFile) == false)
		{
			::printf("Error: Not able to create the following file: %S\n", pathMerge);
			return false;
		}
	}
	else
	{
		if (this->fileExists(pathMerge) == false)
		{
			::printf("Error: This file does not exist: %S\n", pathMerge);
			return false;
		}
	}

	HANDLE fileOne = ::CreateFile(pathOne,
		GENERIC_READ,
		0,
		NULL,               
		OPEN_EXISTING,         
		FILE_ATTRIBUTE_NORMAL,   
		NULL);
	if (fileOne == INVALID_HANDLE_VALUE)
	{
		::printf("Error: Could not open file: %S\n", pathOne);
		return false;
	}

	HANDLE fileTwo = ::CreateFile(pathTwo,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (fileTwo == INVALID_HANDLE_VALUE)
	{
		::printf("Error: Could not open file: %S\n", pathTwo);
		return false;
	}

	HANDLE fileMerge = ::CreateFile(pathMerge,
		FILE_APPEND_DATA,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (fileMerge == INVALID_HANDLE_VALUE)
	{
		::printf("Error: Could not open file: %S\n", pathMerge);
		return false;
	}

	DWORD bytesRead = 0;
	DWORD bytesWritten = 0;
	DWORD pos = 0;
	BYTE* fileOneBuff = (BYTE*)::malloc(1024 * 1024);
	while (::ReadFile(fileOne, fileOneBuff, sizeof(fileOneBuff), &bytesRead, NULL) && bytesRead > 0)
	{
		pos = ::SetFilePointer(fileMerge, 0, NULL, FILE_END);
		::WriteFile(fileMerge, fileOneBuff, bytesRead, &bytesWritten, NULL);
	}
	::free(fileOneBuff);

	bytesRead = 0;
	bytesWritten = 0;
	pos = 0;
	BYTE* fileTwoBuff = (BYTE*)::malloc(1024 * 1024);
	while (::ReadFile(fileTwo, fileTwoBuff, sizeof(fileTwoBuff), &bytesRead, NULL) && bytesRead > 0)
	{
		pos = ::SetFilePointer(fileMerge, 0, NULL, FILE_END);
		::WriteFile(fileMerge, fileTwoBuff, bytesRead, &bytesWritten, NULL);
	}
	::free(fileTwoBuff);

	::CloseHandle(fileOne);
	::CloseHandle(fileTwo);
	::CloseHandle(fileMerge);

	return true;
}

bool CreateCopyDelete::setDirectoryReadOnly(const std::wstring* dir, const bool value) const
{
	HANDLE hSearch;
	WIN32_FIND_DATA itemData;
	std::wstring searchFilter;
	std::wstring subDir;
	std::wstring file;

	searchFilter = *dir;
	searchFilter += L"\\*";

	hSearch = ::FindFirstFile(searchFilter.c_str(), &itemData);
	while (hSearch != INVALID_HANDLE_VALUE)
	{
		if (wcscmp(itemData.cFileName, L".") != 0 && wcscmp(itemData.cFileName, L"..") != 0)
		{
			if ((itemData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			{
				subDir.clear();
				subDir = *dir;
				subDir += L"\\";
				subDir += itemData.cFileName;

				this->setDirectoryReadOnly(&subDir, value);
			}
			else
			{
				file.clear();
				file = *dir;
				file += L"\\";
				file += itemData.cFileName;

				this->setReadOnly(file.c_str(), value);
			}
		}

		if (::FindNextFile(hSearch, &itemData) == 0)
		{
			::FindClose(hSearch);
			hSearch = INVALID_HANDLE_VALUE;
		}
	}

	return true;
}

bool CreateCopyDelete::setDirReadOnly(const WCHAR* dir, const bool value) const
{
	if (this->directoryExists(dir) == true)
	{
		std::wstring inDir = dir;
		if (value == true)
		{
			::printf("Setting read only flag to all files in directory '%S'\n", inDir.c_str());
		}
		else
		{
			::printf("Clearing read only flag to all files in directory '%S'\n", inDir.c_str());
		}
		return this->setDirectoryReadOnly(&inDir, value);
	}
	else
	{
		::printf("Error: This directory does not exist: %S\n", dir);
		return false;
	}
}

bool CreateCopyDelete::getDirectoryFilePaths(const std::wstring* dir, std::vector<std::wstring>* paths) const
{
	HANDLE hSearch;
	WIN32_FIND_DATA itemData;
	std::wstring searchFilter;
	std::wstring subDir;
	std::wstring file;

	searchFilter = *dir;
	searchFilter += L"\\*";

	hSearch = ::FindFirstFile(searchFilter.c_str(), &itemData);
	while (hSearch != INVALID_HANDLE_VALUE)
	{
		if (wcscmp(itemData.cFileName, L".") != 0 && wcscmp(itemData.cFileName, L"..") != 0)
		{
			if ((itemData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			{
				subDir.clear();
				subDir = *dir;
				subDir += L"\\";
				subDir += itemData.cFileName;

				this->getDirectoryFilePaths(&subDir, paths);
			}
			else
			{
				file.clear();
				file = *dir;
				file += L"\\";
				file += itemData.cFileName;

				paths->push_back(file);
			}
		}

		if (::FindNextFile(hSearch, &itemData) == 0)
		{
			::FindClose(hSearch);
			hSearch = INVALID_HANDLE_VALUE;
		}
	}

	return true;
}

bool CreateCopyDelete::getDirFilePaths(const WCHAR* dir, std::vector<std::wstring>* paths) const
{
	if (this->directoryExists(dir) == true)
	{
		std::wstring inDir = dir;
		return this->getDirectoryFilePaths(&inDir, paths);
	}
	else
	{
		::printf("Error: This directory does not exist: %S\n", dir);
		return false;
	}
}

bool CreateCopyDelete::printDirectoryPaths(const std::wstring* dir, const bool recursive) const
{
	HANDLE hSearch;
	WIN32_FIND_DATA itemData;
	std::wstring searchFilter;
	std::wstring subDir;
	std::wstring file;

	searchFilter = *dir;
	searchFilter += L"\\*";

	hSearch = ::FindFirstFile(searchFilter.c_str(), &itemData);
	while (hSearch != INVALID_HANDLE_VALUE)
	{
		if (wcscmp(itemData.cFileName, L".") != 0 && wcscmp(itemData.cFileName, L"..") != 0)
		{
			if ((itemData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			{
				subDir.clear();
				subDir = *dir;
				subDir += L"\\";
				subDir += itemData.cFileName;

				std::wcout << subDir << std::endl;

				if (recursive == true)
				{
					this->printDirectoryPaths(&subDir, recursive);
				}
			}
			else
			{
				file.clear();
				file = *dir;
				file += L"\\";
				file += itemData.cFileName;

				std::wcout << file << std::endl;
			}
		}

		if (::FindNextFile(hSearch, &itemData) == 0)
		{
			::FindClose(hSearch);
			hSearch = INVALID_HANDLE_VALUE;
		}
	}

	return true;
}

bool CreateCopyDelete::printDirPaths(const WCHAR* dir, const bool recursive) const
{
	if (this->directoryExists(dir) == true)
	{
		std::wstring inDir = dir;
		return this->printDirectoryPaths(&inDir, recursive);
	}
	else
	{
		::printf("Error: This directory does not exist: %S\n", dir);
		return false;
	}
}

bool CreateCopyDelete::printFileTime(const WCHAR* path) const
{
	if (this->fileExists(path) == false)
	{
		::printf("Error: This file does not exist: %S\n", path);
		return false;
	}

	HANDLE handle = ::CreateFile(
		path,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (handle == INVALID_HANDLE_VALUE)
	{
		DWORD err = ::GetLastError();
		::printf("Error: %d\n", err);
		return false;
	}
	else
	{
		FILETIME timeCreated, timeModified, timeAccessed;
		SYSTEMTIME utc, local;

		if (::GetFileTime(handle, &timeCreated, &timeAccessed, &timeModified) == 0)
		{
			::printf("Not able to get the file time info\n");
			return false;
		}

		if (::FileTimeToSystemTime(&timeCreated, &utc) == 0)
		{
			::printf("File time to system time conversion error\n");
			return false;
		}
		if (::SystemTimeToTzSpecificLocalTime(NULL, &utc, &local) == 0)
		{
			::printf("System time to local time conversion error\n");
			return false;
		}
		::printf("File created on: %02d/%02d/%d %02d:%02d:%02d\n", local.wDay, local.wMonth, local.wYear, local.wHour, local.wMinute, local.wSecond);
		if (::FileTimeToSystemTime(&timeModified, &utc) == 0)
		{
			::printf("File time to system time conversion error\n");
			return false;
		}
		if (::SystemTimeToTzSpecificLocalTime(NULL, &utc, &local) == 0)
		{
			::printf("System time to local time conversion error\n");
			return false;
		}
		::printf("File modified on: %02d/%02d/%d %02d:%02d:%02d\n", local.wDay, local.wMonth, local.wYear, local.wHour, local.wMinute, local.wSecond);
		if (::FileTimeToSystemTime(&timeAccessed, &utc) == 0)
		{
			::printf("File time to system time conversion error\n");
			return false;
		}
		if (::SystemTimeToTzSpecificLocalTime(NULL, &utc, &local) == 0)
		{
			::printf("System time to local time conversion error\n");
			return false;
		}
		::printf("File accessed on: %02d/%02d/%d %02d:%02d:%02d\n", local.wDay, local.wMonth, local.wYear, local.wHour, local.wMinute, local.wSecond);

		::CloseHandle(handle);
		return true;
	}
}

bool CreateCopyDelete::getDiskCapacityInfo(const WCHAR* path, uint64_t* freeBytesAvailableToCaller, uint64_t* totalNumberOfBytes, uint64_t* totalNumberOfFreeBytes) const
{
	ULARGE_INTEGER FreeBytesAvailableToCaller = {};
	ULARGE_INTEGER TotalNumberOfBytes = {};
	ULARGE_INTEGER TotalNumberOfFreeBytes = {};
	if (::GetDiskFreeSpaceEx(path, &FreeBytesAvailableToCaller, &TotalNumberOfBytes, &TotalNumberOfFreeBytes) == 0)
	{
		::printf("Error: Not able to get the disk capacity info");
		return false;
	}
	else
	{
		*freeBytesAvailableToCaller = static_cast<uint64_t>(FreeBytesAvailableToCaller.QuadPart);
		*totalNumberOfBytes = static_cast<uint64_t>(TotalNumberOfBytes.QuadPart);
		*totalNumberOfFreeBytes = static_cast<uint64_t>(TotalNumberOfFreeBytes.QuadPart);
		::printf("Drive space info:\n");
		::printf("FreeBytesAvailableToCaller: %" PRIu64 "\n", *freeBytesAvailableToCaller);
		::printf("TotalNumberOfBytes: %" PRIu64 "\n", *totalNumberOfBytes);
		::printf("TotalNumberOfFreeBytes: %" PRIu64 "\n", *totalNumberOfFreeBytes);
		return true;
	}
}

uint32_t CreateCopyDelete::getDriveType(const WCHAR* path) const
{
	uint32_t driveType = static_cast<uint32_t>(::GetDriveType(path));
	switch (driveType)
	{
	case 0:
		::printf("Drive type info: The drive type cannot be determined.\n");
		break;
	case 1:
		::printf("Drive type info: The root path is invalid; for example, there is no volume mounted at the specified path..\n");
		break;
	case 2:
		::printf("Drive type info: The drive has removable media; for example, a floppy drive, thumb drive, or flash card reader.\n");
		break;
	case 3:
		::printf("Drive type info: The drive has fixed media; for example, a hard disk drive or flash drive..\n");
		break;
	case 4:
		::printf("Drive type info: The drive is a remote (network) drive.\n");
		break;
	case 5:
		::printf("Drive type info: The drive is a CD-ROM drive.\n");
		break;
	case 6:
		::printf("Drive type info: The drive is a RAM disk.\n");
		break;
	default:
		::printf("Drive type info: The drive type is unknown.\n");
		break;
	}
	return driveType;
}