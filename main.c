#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int compare(char* str1, char* str2, unsigned int smallerLength) {
    printf("Comparing %s and %s\n\n", str1, str2);

    int counter = 0;
    for (int i = 0; i < smallerLength; i++) {
        if (str1[i] == str2[i]) {
            counter++;
        }
    }

    return counter;
}

int main(int argc, char** argv) {

    int f1 = open(argv[1], O_RDONLY);
    int f2 = open(argv[2], O_RDONLY);

    struct stat stat_file1;
    struct stat stat_file2;

    if (stat(argv[1], &stat_file1) == -1) {
        return 0;
    } else if (stat(argv[2], &stat_file2) == -1) {
        return 0;
    }

    char *buffer1 = (char *) malloc(sizeof(char) * stat_file1.st_size);
    char *buffer2 = (char *) malloc(sizeof(char) * stat_file2.st_size);
    read(f1, buffer1, stat_file1.st_size);
    read(f2, buffer2, stat_file2.st_size);

    unsigned int smaller = (strlen(buffer1) <= strlen(buffer2)) ? strlen(buffer1) : strlen(buffer2);
    unsigned int bigger = strlen(buffer1) + strlen(buffer2) - smaller;
    unsigned int diff = bigger - smaller;

    int i;
    for (i = 0; i <= diff; i++) {
        char* buffer;

        if (stat_file1.st_size >= stat_file2.st_size) {
            lseek(f1, i, SEEK_SET);
            buffer = (char *) malloc(bigger - i);
            read(f1, buffer, bigger - i);

            if (compare(buffer, buffer2, smaller) >= smaller / 2) {
                return 3;
            }
        } else {
            lseek(f2, i, SEEK_SET);
            buffer = (char *) malloc(bigger - i);
            read(f2, buffer, bigger - i);

            if (compare(buffer1, buffer, smaller) >= smaller / 2) {
                return 3;
            }
        }
    }

    if (smaller >= 2) {
        unsigned int j;
        int k;

        for (j = smaller - 1; j >= 1; j--) {
            char* buffer;

            if (stat_file1.st_size <= stat_file2.st_size) {
                lseek(f1, j, SEEK_SET);
                buffer = (char *) malloc(smaller - j);
                read(f1, buffer, smaller - j);

                if (compare(buffer, buffer2, smaller - j) >= (smaller + 1) / 2) {
                    return 3;
                }
            } else {
                lseek(f2, j, SEEK_SET);
                buffer = (char *) malloc(smaller -j);
                read(f2, buffer, smaller - j);

                if (compare(buffer, buffer1, smaller - j) >= (smaller + 1) / 2) {
                    return 3;
                }
            }
        }

        for (j = smaller - 1, k = bigger - 1; j >= 1; j--, k--) {
            char* buffer;
            char small[smaller];
            if (stat_file1.st_size > stat_file2.st_size) {
                lseek(f1, k, SEEK_SET);
                buffer = (char *) malloc(bigger - k);
                read(f1, buffer, bigger - k);

                strncpy(small, buffer2, smaller - j);
                small[smaller - j] = 0;

                if (compare(buffer, small, smaller - j) >= (smaller + 1) / 2) {
                    return 3;
                }
            } else {
                lseek(f2, k, SEEK_SET);
                buffer = (char *) malloc(bigger -k);
                read(f2, buffer, bigger - k);

                strncpy(small, buffer1, smaller - j);
                small[smaller - j] = 0;

                if (compare(buffer, small, smaller - j) >= (smaller + 1) / 2) {
                    return 3;
                }
            }
        }
    }
    return 2;
}
