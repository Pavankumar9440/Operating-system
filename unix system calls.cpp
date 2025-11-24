/* Demonstration of UNIX file management system calls:
   open, read, write, lseek, close
   Compile (Linux/WSL):
       gcc unix_file_syscalls.c -o unix_file_syscalls
   Run:
       ./unix_file_syscalls
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>    // for open() and flags
#include <unistd.h>   // for read(), write(), close(), lseek()
#include <sys/stat.h> // for file permissions

#define BUF_SIZE 1024

int main(void) {
    char src[256], dest[256];
    int fd_src, fd_dest;
    ssize_t nread;
    char buffer[BUF_SIZE];

    printf("Enter source file name: ");
    scanf("%255s", src);

    printf("Enter destination file name: ");
    scanf("%255s", dest);

    /* open source file in read-only mode */
    fd_src = open(src, O_RDONLY);
    if (fd_src == -1) {
        perror("Error opening source file");
        return 1;
    }

    /* open/create destination file in write-only mode, truncate if exists
       permissions: rw-r--r-- (0644) */
    fd_dest = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dest == -1) {
        perror("Error opening/creating destination file");
        close(fd_src);
        return 1;
    }

    printf("\nSource file descriptor      = %d\n", fd_src);
    printf("Destination file descriptor = %d\n", fd_dest);

    /* copy loop using read() and write() */
    while ((nread = read(fd_src, buffer, BUF_SIZE)) > 0) {
        ssize_t nwritten = write(fd_dest, buffer, nread);
        if (nwritten == -1) {
            perror("Error writing to destination file");
            close(fd_src);
            close(fd_dest);
            return 1;
        }
    }

    if (nread == -1) {
        perror("Error reading from source file");
        close(fd_src);
        close(fd_dest);
        return 1;
    }

    /* use lseek to find size of destination file (move to end) */
    off_t size = lseek(fd_dest, 0, SEEK_END);
    if (size == (off_t)-1) {
        perror("lseek error");
    } else {
        printf("Copy completed. Size of '%s' = %ld bytes\n", dest, (long)size);
    }

    /* close both files */
    close(fd_src);
    close(fd_dest);

    printf("Files closed successfully.\n");
    return 0;
}

