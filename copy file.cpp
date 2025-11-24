#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int src, dest;
    char buffer[100];
    int bytesRead;

    src = open("input.txt", O_RDONLY);
    if (src < 0) {
        printf("Error: Cannot open input file.\n");
        return 1;
    }


    dest = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest < 0) {
        printf("Error: Cannot open/create output file.\n");
        close(src);
        return 1;
    }

 
    while ((bytesRead = read(src, buffer, sizeof(buffer))) > 0) {
        write(dest, buffer, bytesRead);
    }

    printf("File copied successfully.\n");

  
    close(src);
    close(dest);

    return 0;
}

