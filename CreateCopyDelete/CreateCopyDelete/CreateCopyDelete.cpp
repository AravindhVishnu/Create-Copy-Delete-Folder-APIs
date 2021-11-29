#include "CreateCopyDelete.h"

CreateCopyDelete::CreateCopyDelete()
{
}

CreateCopyDelete::~CreateCopyDelete()
{
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
    printf("Directory '%S' already exists\n", newDir.c_str());
    return false;
  }
  else
  {
    printf("Creating directory '%S'\n", newDir.c_str());

    if (::CreateDirectory(newDir.c_str(), NULL) == 0)
    {
      printf("Failed to create: %S\n", newDir.c_str());
      return false;
    }

    return true;
  }
}

bool CreateCopyDelete::copyDir(const WCHAR* fromDir, const WCHAR* toDir)
{
  if (this->directoryExists(fromDir) == true)
  {
    std::wstring inDir = fromDir;
    std::wstring outDir = toDir;
    printf("Copying directory from '%S' to '%S'\n", inDir.c_str(), outDir.c_str());
    return this->copyDirectory(&inDir, &outDir);
  }
  else
  {
    printf("This directory does not exist: %S\n", fromDir);
    return false;
  }
}

bool CreateCopyDelete::deleteDir(const WCHAR* dir)
{
  if (this->directoryExists(dir) == true)
  {
    std::wstring inDir = dir;
    printf("Deleting directory from '%S'\n", inDir.c_str());
    return this->deleteDirectory(&inDir);
  }
  else
  {
    printf("This directory does not exist: %S\n", dir);
    return false;
  }
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

  std::wstring filter = L".";

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