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

    createCopyDelete.setReadOnly(L"C:\\temp\\toCopy1\\test3.txt", true);

    createCopyDelete.setReadOnly(L"C:\\temp\\toCopy1\\test3.txt", false);

    uint32_t size = 0;
    createCopyDelete.getDirSize(L"C:\\temp\\toCopy1", &size);
    printf("Directory size: %d\n", size);

    const WCHAR str[] = L"Example text testing fileWrite\n";
    createCopyDelete.fileWrite(L"C:\\temp\\toCopy1\\test3.txt", str);

    const WCHAR strAppend[] = L"Example text testing fileAppendWrite\n";
    createCopyDelete.fileAppendWrite(L"C:\\temp\\toCopy1\\test3.txt", strAppend);

    size = 0;
    createCopyDelete.getFileSize(L"C:\\temp\\toCopy1\\test3.txt", &size);
    printf("File size: %d\n", size);

    WCHAR* strRead = (WCHAR*)malloc(size);
    createCopyDelete.fileRead(L"C:\\temp\\toCopy1\\test3.txt", strRead);
    printf("Testing fileRead: %ws\n", strRead);
    free(strRead);

    createCopyDelete.deleteFile(L"C:\\temp\\toCopy1", L"test3.txt");

    createCopyDelete.deleteDir(L"C:\\temp\\toCopy3");

    createCopyDelete.deleteDir(L"C:\\temp\\toCopy1");

    createCopyDelete.deleteDir(L"C:\\temp\\fromCopy");

    system("pause");

    return 0;
}