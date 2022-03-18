# Create-Copy-Delete
This repository contains Windows file system C++ APIs and an example console application.

Description: Includes APIs to:
- Create, delete, copy and move files and folders. Folders are copied, moved and deleted recursively.
- Get file size and folder size
- Write, append write and read file operation
- Set and clear read only flag file operation

The implementation is based on std::wstring and WCHAR which are needed to access and create files with 
unicode characters.

Tools: Visual Studio 2022

Note: Tested only on Windows 10
