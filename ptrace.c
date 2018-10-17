#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <syscall.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    pid_t child;
    long orig_eax;
    child = fork();
    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        //execl("/bin/ls", "ls", NULL);
        char *cmd_argv[] ={
            "ls", "--color=auto", "-l", ".", NULL
        };

        execv("/bin/ls", cmd_argv);
    }
    else {
        wait(NULL);
        errno = 0;
        orig_eax = ptrace(PTRACE_PEEKUSER, child, 8*ORIG_RAX, NULL);
        if (orig_eax < 0)
            perror("ptrace");
        printf("The child made a "
                "system call %ld\n", orig_eax);
        ptrace(PTRACE_CONT, child, NULL, NULL);
        
    }
    return 0;
}

