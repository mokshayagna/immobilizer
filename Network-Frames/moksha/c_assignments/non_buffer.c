#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd;
    char buffer[200];
    int bytes;

    // 1. Open the file (unbuffered)
    fd = open("data.txt", O_RDONLY);

    // 2. Check for errors
    if (fd == -1) {
        printf("[ERROR] Could not open file\n");
        return -1;
    }

    printf("[INFO] File content (unbuffered):\n");

    // 3. Read until no more bytes (EOF)
    while ((bytes = read(fd, buffer, sizeof(buffer)-1)) > 0) {
        buffer[bytes] = '\0';   // terminate string properly
        printf("%s", buffer);   // print what we read
    }

    // 4. Close the file
    close(fd);

    return 0;
}
