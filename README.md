# Create-Copy-Delete
This repository contains Windows file system C++ APIs and an example console application to test the APIs.

Description: Includes APIs to:
- Create, delete, copy and move files and folders. Folders are copied, moved and deleted recursively.
- Get file size and folder size. Folder size is retrieved recursively.
- Set and clear read only flag on folders and files. Setting read only flag on a folder is done recursively.
- Get list of all file paths in a folder. This operation is done recursively.
- Write, append write, read and merge file operation.
- Print all file paths and subfolder paths in a folder.
- Print file time info.
- Get drive space and drive type info.
- Zip and unzip operations.
- Add file to zip archive.
- Remove file from zip archive.

Windows file systems NTFS/FAT32 stores file names in unicode.
The implementation is based on std::wstring and WCHAR which are needed to access and create files with 
unicode characters.

Tools: Visual Studio 2022

Note: Tested only on Windows 10
