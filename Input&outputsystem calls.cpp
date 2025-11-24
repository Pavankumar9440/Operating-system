#include <stdio.h>
#include <stdlib.h>
#include <io.h>         // _open, _read, _write, _close, _lseek
#include <fcntl.h>      // _O_RDWR, _O_CREAT, etc.
#include <sys/stat.h>   // _stat
#include <string.h>     // strlen, memset, sprintf
#include <windows.h>    // FindFirstFile, FindNextFile, WIN32_FIND_DATA

int main() {
    char filename[260];
    char dirpath[260];
    int fd;
    int n;
    char buffer[100];
    struct _stat st;

    printf("=== I/O System Call Demo (Windows / Dev-C++) ===\n\n");

    /* ---------- Part 1: File operations (open, write, seek, read, stat) ---------- */

    printf("Enter file name to create/open: ");
    scanf("%255s", filename);

    /* _open() : open the file (create if not exists) with read-write permissions */
    fd = _open(filename, _O_RDWR | _O_CREAT, _S_IREAD | _S_IWRITE);
    if (fd == -1) {
        perror("_open");
        return 1;
    }
    printf("\n[_open] Opened file '%s' successfully.\n", filename);

    /* _write() : write some sample text into the file */
    const char *text = "Hello, this is sample text written using _write().\n";
    n = _write(fd, text, strlen(text));
    if (n == -1) {
        perror("_write");
        _close(fd);
        return 1;
    }
    printf("[_write] Wrote %d bytes into the file.\n", n);

    /* _lseek() : move file offset to the beginning */
    if (_lseek(fd, 0L, SEEK_SET) == -1L) {
        perror("_lseek");
        _close(fd);
        return 1;
    }
    printf("[_lseek] File offset moved to the beginning.\n");

    /* _read() : read some bytes from the file */
    memset(buffer, 0, sizeof(buffer));
    n = _read(fd, buffer, sizeof(buffer) - 1);
    if (n == -1) {
        perror("_read");
        _close(fd);
        return 1;
    }
    printf("[_read] Read %d bytes from the file:\n", n);
    printf("        \"%s\"\n", buffer);

    /* _stat() : get information about the file */
    if (_stat(filename, &st) == -1) {
        perror("_stat");
        _close(fd);
        return 1;
    }
    printf("\n[_stat] Information about file '%s':\n", filename);
    printf("  File size : %ld bytes\n", (long)st.st_size);
    printf("  Mode      : %d\n", st.st_mode);
    printf("  Links     : %ld\n", (long)st.st_nlink);

    /* ---------- Part 2: Directory operations (FindFirstFile, FindNextFile) ---------- */

    printf("\nEnter directory path to list (e.g. C:\\ or .): ");
    scanf("%255s", dirpath);

    char searchPattern[300];
    /* Create pattern like: C:\*  or  .\*  */
    sprintf(searchPattern, "%s\\*", dirpath);

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPattern, &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error: Could not open directory.\n");
        _close(fd);
        return 1;
    }

    printf("\n[FindFirstFile / FindNextFile] Directory entries in '%s':\n", dirpath);
    do {
        printf("  %s\n", findData.cFileName);
    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);

    /* Close file */
    if (_close(fd) == -1) {
        perror("_close");
        return 1;
    }

    printf("\nAll operations completed successfully.\n");
    return 0;
}

