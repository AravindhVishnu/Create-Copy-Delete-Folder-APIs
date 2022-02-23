#include "CreateCopyDelete.h"

CreateCopyDelete::CreateCopyDelete()
{
}

CreateCopyDelete::~CreateCopyDelete()
{
}

bool CreateCopyDelete::fileExists(const WCHAR* path)
{
	DWORD attributes = GetFileAttributes(path);
	return (attributes != INVALID_FILE_ATTRIBUTES &&
		!(attributes & FILE_ATTRIBUTE_DIRECTORY));
}

bool CreateCopyDelete::directoryExists(const WCHAR* path)
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

bool CreateCopyDelete::createDir(const WCHAR* dir)
{
	std::wstring newDir = dir;

	if (this->directoryExists(dir) == true)
	{
		printf("Error: Directory '%S' already exists\n", newDir.c_str());
		return false;
	}
	else
	{
		printf("Creating directory '%S'\n", newDir.c_str());

		if (::CreateDirectory(newDir.c_str(), NULL) == 0)
		{
			printf("Error: Failed to create: %S\n", newDir.c_str());
			return false;
		}

		return true;
	}
}

bool CreateCopyDelete::copyDir(const WCHAR* fromDir, const WCHAR* toDir)
{
	if (this->directoryExists(fromDir) == false)
	{
		printf("Error: This directory does not exist: %S\n", fromDir);
		return false;
	}
	if (this->directoryExists(toDir) == true)
	{
		printf("Error: This directory already exists: %S\n", toDir);
		return false;
	}

	std::wstring inDir = fromDir;
	std::wstring outDir = toDir;
	printf("Copying directory from '%S' to '%S'\n", inDir.c_str(), outDir.c_str());
	return this->copyDirectory(&inDir, &outDir);
}

bool CreateCopyDelete::deleteDir(const WCHAR* dir)
{
	if (this->directoryExists(dir) == true)
	{
		std::wstring inDir = dir;
		printf("Deleting directory '%S'\n", inDir.c_str());
		return this->deleteDirectory(&inDir);
	}
	else
	{
		printf("Error: This directory does not exist: %S\n", dir);
		return false;
	}
}

bool CreateCopyDelete::moveDir(const WCHAR* fromDir, const WCHAR* toDir)
{
	return this->copyDir(fromDir, toDir) && this->deleteDir(fromDir);
}

bool CreateCopyDelete::copyDirectory(std::wstring* fromDir, std::wstring* toDir)
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

bool CreateCopyDelete::deleteDirectory(std::wstring* dir)
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

bool CreateCopyDelete::createFile(const WCHAR* dir, const WCHAR* file)
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
			printf("Error: %d\n", err);
			return false;
		}
		else
		{
			::CloseHandle(handle);
			printf("Creating file '%S'\n", path.c_str());
			return true;
		}
	}
	else
	{
		printf("Error: This directory does not exist: %S\n", dir);
		return false;
	}
}

bool CreateCopyDelete::copyFile(const WCHAR* fromDir, const WCHAR* toDir, const WCHAR* file)
{
	if (this->directoryExists(fromDir) == false)
	{
		printf("Error: This directory does not exist: %S\n", fromDir);
		return false;
	}
	if (this->directoryExists(toDir) == false)
	{
		printf("Error: This directory does not exist: %S\n", toDir);
		return false;
	}

	std::wstring fromDirectory = fromDir;
	std::wstring fileCopy = file;
	std::wstring doubleBackslash(L"\\");

	std::wstring fromPath = fromDirectory.append(doubleBackslash);
	fromPath = fromPath.append(file);

	if (this->fileExists(fromPath.c_str()) == false)
	{
		printf("Error: This file does not exist: %S\n", fromPath.c_str());
		return false;
	}

	std::wstring toDirectory = toDir;
	std::wstring toPath = toDirectory.append(doubleBackslash);
	toPath = toPath.append(file);

	if (this->fileExists(toPath.c_str()) == true)
	{
		printf("Error: This file already exists: %S\n", toPath.c_str());
		return false;
	}

	if (::CopyFile(fromPath.c_str(), toPath.c_str(), FALSE) == 0)
	{
		DWORD err = GetLastError();
		printf("Error: %d\n", err);
		return false;
	}
	else
	{
		printf("Copying file from '%S' to '%S'\n", fromPath.c_str(), toPath.c_str());
		return true;
	}
}

bool CreateCopyDelete::deleteFile(const WCHAR* dir, const WCHAR* file)
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
				DWORD err = GetLastError();
				printf("Error: %d\n", err);
				return false;
			}
			else
			{
				printf("Deleting file '%S'\n", path.c_str());
				return true;
			}
		}
		else
		{
			printf("Error: This file does not exist: %S\n", path.c_str());
			return false;
		}
	}
	else
	{
		printf("Error: This directory does not exist: %S\n", dir);
		return false;
	}
}

bool CreateCopyDelete::moveFile(const WCHAR* fromDir, const WCHAR* toDir, const WCHAR* file)
{
	return this->copyFile(fromDir, toDir, file) && this->deleteFile(fromDir, file);
}