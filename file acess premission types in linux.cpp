#include <windows.h>
#include <stdio.h>

int main() {
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    char filename[MAX_PATH];

    printf("Enter file name (with path if needed): ");
    scanf("%s", filename);

    if (!GetFileAttributesEx(filename, GetFileExInfoStandard, &fileInfo)) {
        printf("Error: Cannot access file '%s'\n", filename);
        return 1;
    }

    printf("\nFile: %s\n", filename);

    DWORD attr = fileInfo.dwFileAttributes;

    if (attr & FILE_ATTRIBUTE_DIRECTORY)
        printf("File Type: Directory\n");
    else
        printf("File Type: Regular File\n");

    printf("Attributes: ");
    if (attr & FILE_ATTRIBUTE_READONLY)
        printf("Read-Only ");
    else
        printf("Read/Write ");

    if (attr & FILE_ATTRIBUTE_HIDDEN)
        printf("Hidden ");
    if (attr & FILE_ATTRIBUTE_SYSTEM)
        printf("System ");
    if (attr & FILE_ATTRIBUTE_ARCHIVE)
        printf("Archive ");
    printf("\n");

    LARGE_INTEGER size;
    size.HighPart = fileInfo.nFileSizeHigh;
    size.LowPart = fileInfo.nFileSizeLow;
    printf("File Size: %lld bytes\n", size.QuadPart);

    FILETIME ftWrite;
    SYSTEMTIME stUTC, stLocal;
    ftWrite = fileInfo.ftLastWriteTime;

    FileTimeToSystemTime(&ftWrite, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    printf("Last Modified: %02d/%02d/%d %02d:%02d:%02d\n",
           stLocal.wDay, stLocal.wMonth, stLocal.wYear,
           stLocal.wHour, stLocal.wMinute, stLocal.wSecond);

    return 0;
}

