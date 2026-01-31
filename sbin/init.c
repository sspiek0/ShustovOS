#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/reboot.h>
#include <errno.h>
#include <string.h>

int main() {
    printf("[ShustovOS]: mounting /proc\n");
    if (mount("proc", "/proc", "proc", 0, NULL) < 0) 
        printf("[ShustovOS]: error mounting /proc: %s\n", strerror(errno));

    printf("[ShustovOS]: mounting /sys\n");
    if (mount("sysfs", "/sys", "sysfs", 0, NULL) < 0) 
        printf("[ShustovOS]: error mounting /sys: %s\n", strerror(errno));

    printf("[ShustovOS]: mounting /dev\n");
    if (mount("devtmpfs", "/dev", "devtmpfs", 0, NULL) < 0) 
        printf("[ShustovOS]: error mounting /dev: %s\n", strerror(errno));
    
    while(1) {
        printf("[ShustovOS]: forking for greeter.py...\n");
        pid_t pid = fork();

        if (pid < 0) {
            printf("[ShustovOS]: fork failed: %s\n", strerror(errno));
            sleep(1);
            continue;
        }

        if (pid == 0) {
            printf("[ShustovOS]: child process started, setting sid\n");
            setsid();

            printf("[ShustovOS]: opening /dev/console\n");
            int fd = open("/dev/console", O_RDWR); 
            if (fd >= 0) {
                printf("[ShustovOS]: console opened, setting controlling tty\n");
                ioctl(fd, TIOCSCTTY, 1);
                dup2(fd, 0); dup2(fd, 1); dup2(fd, 2);
                if (fd > 2) close(fd);
            } else {
                printf("[ShustovOS]: failed to open /dev/console: %s\n", strerror(errno));
            }

            printf("[ShustovOS]: executing /bin/greeter.py\n");
            char *args[] = {"/usr/bin/python3.14", "/bin/greeter.py", NULL}; 
            execv("/usr/bin/python3.14", args);

            printf("[ShustovOS]: execv failed: %s\n", strerror(errno));
            exit(1);
        } else {
            int status;
            printf("[ShustovOS]: waiting for greeter.py (PID: %d)...\n", pid);
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status)) {
                printf("[ShustovOS]: greeter.py exited with status %d\n", WEXITSTATUS(status));
            } else {
                printf("[ShustovOS]: greeter.py greeter.pyinated abnormally\n");
            }

            printf("[ShustovOS]: shutting down...\n");
            reboot(RB_POWER_OFF);
        }
    }
    return 0;
}