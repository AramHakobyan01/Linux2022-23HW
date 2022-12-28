#ifndef FIRST_PROJECT_LOGGING_H
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void quick(int er) {
    exit(er);
}

void cp(int argc, char** argv) {
   if (argc != 3) {
        quick(errno);
    }
    int file = open(argv[1] , O_RDONLY);
    if (file < 0) {
        std::cerr << file;
        quick(errno);
    }
    char* buffer[50];
    size_t r;
    int file2 = open(argv[2], O_CREAT | O_WRONLY);
    if(file2 < 0) {
        std::cerr << file2;
        quick(errno);
    }
    while (r = read(file,buffer,sizeof(buffer))) {
        write(file2,buffer,r);
    }
}
#define FIRST_PROJECT_LOGGING_H

#endif //FIRST_PROJECT_LOGGING_H
