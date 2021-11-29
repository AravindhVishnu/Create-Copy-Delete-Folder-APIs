#include "CreateCopyDelete.h"

int main(int argc, char** argv)
{
    printf("************************************************************************\n");
    printf("   Create, copy and delete folders");
    printf("   Build time: %s %s\n", __DATE__, __TIME__);
    printf("************************************************************************\n");

    CreateCopyDelete createCopyDelete;

    createCopyDelete.createDir(L"C:\\Temp\\fromCopy");

    createCopyDelete.copyDir(L"C:\\Temp\\fromCopy", L"C:\\Temp\\toCopy");

    createCopyDelete.deleteDir(L"C:\\Temp\\fromCopy");

    createCopyDelete.deleteDir(L"C:\\Temp\\toCopy");

    system("pause");

    return 0;
}