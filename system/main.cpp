#include <sys/wait.h>
#include <unistd.h>

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

int main()
{
    char *args[3];

    char *program[4];
    program[0] = "ls";
    program[1] = "-a";
    program[2] = "-l";
    program[3] = nullptr;
    do_command(program);
    program[0] = "cat";
    program[1] = "/etc/lsb-release";
    program[2] = nullptr;
    do_command(program);
    return 0;
}