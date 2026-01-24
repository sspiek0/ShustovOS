#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mount.h>

int main() {
    mount("proc", "/proc", "proc", 0, NULL);
    mount("sysfs", "/sys", "sysfs", 0, NULL);
    mount("devtmpfs", "/dev", "devtmpfs", 0, NULL);

    printf("--- MicroOS Initialized ---\n");

    while(1) {
        pid_t pid = fork();
        if (pid == 0) {
            char *args[] = {"/bin/term", NULL};
            execv("/bin/term", args);
            exit(1);
        } else {
            int status;
            wait(&status);
            char *args[] = {"/bin/poweroff", NULL};
            execv("/bin/poweroff", args);
        }
    }
    return 0;
}