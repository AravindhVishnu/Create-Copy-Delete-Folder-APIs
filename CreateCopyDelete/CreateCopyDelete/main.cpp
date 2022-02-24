#include "CreateCopyDelete.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    
    printf("**********************************************************************************\n");
    printf("   Create, copy and delete folders and files");
    printf("   Build time: %s %s\n", __DATE__, __TIME__);
    printf("**********************************************************************************\n");

    CreateCopyDelete createCopyDelete;

    createCopyDelete.createDir(L"C:\\temp\\fromCopy");

    createCopyDelete.createFile(L"C:\\temp\\fromCopy", L"test1.txt");

    createCopyDelete.createDir(L"C:\\temp\\fromCopy\\test2");

    createCopyDelete.createFile(L"C:\\temp\\fromCopy\\test2", L"test2.txt");

    createCopyDelete.copyDir(L"C:\\temp\\fromCopy", L"C:\\temp\\toCopy1");

    createCopyDelete.createDir(L"C:\\temp\\toCopy2");

    createCopyDelete.createFile(L"C:\\temp\\toCopy2", L"test3.txt");

    createCopyDelete.createFile(L"C:\\temp\\toCopy2", L"test4.txt");

    createCopyDelete.moveDir(L"C:\\temp\\toCopy2", L"C:\\temp\\toCopy3");

    createCopyDelete.moveFile(L"C:\\temp\\toCopy3", L"C:\\temp\\toCopy1", L"test3.txt");

    createCopyDelete.deleteFile(L"C:\\temp\\toCopy1", L"test3.txt");

    createCopyDelete.deleteDir(L"C:\\temp\\toCopy3");

    createCopyDelete.deleteDir(L"C:\\temp\\toCopy1");

    createCopyDelete.deleteDir(L"C:\\temp\\fromCopy");

    system("pause");

    return 0;
}
