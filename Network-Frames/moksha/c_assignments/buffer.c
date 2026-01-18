#include <stdio.h>

int main() {
    FILE *fp;
    char buffer[100];   // temporary line storage

    // 1. Open file in read mode
    fp = fopen("data.txt", "r");

    // 2. Check if opened successfully
    if (fp == NULL) {
        printf("[ERROR] Could not open file\n");
        return -1;
    }

    printf("[INFO] Reading file contents:\n");

    // 3. Read line by line until end of file
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);   // print the extracted line
    }
    printf("\n");
    // 4. Close file
    fclose(fp);

    return 0;
}
