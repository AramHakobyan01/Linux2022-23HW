#ifndef FIRST_PROJECT_PROCESS_H
#define FIRST_PROJECT_PROCESS_H

int do_command(char** com){
    int status;
    if ( fork() == 0 ) {
        execvp(com[0], com);
        return 1;
    }
    else {
        wait(&status);
        return 0;
    }

}

#endif //FIRST_PROJECT_PROCESS_H
