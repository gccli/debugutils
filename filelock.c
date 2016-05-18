#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <sys/file.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int lock_write_pid(const char *lockf, const char *pidfile)
{
    int  ret, fd;
    char str[16] = {0};

    fd = -1;
    ret = EINVAL;
    do {
        fd = open(lockf, O_CREAT|O_WRONLY|O_TRUNC, 0644);
        if (fd < 0) {
            ret = errno;
            fprintf(stderr, "open(%s) error - %s\n", lockf, strerror(errno));
            break;
        }

        ret = flock(fd, LOCK_EX|LOCK_NB);
        if (ret < 0) {
            ret = errno;
            close(fd);
            fprintf(stderr, "flock(%s) error - %s\n", lockf, strerror(errno));

            break;
        }

        fd = open(pidfile, O_CREAT|O_WRONLY|O_TRUNC, 0644);
        if (fd < 0) {
            ret = errno;
            close(fd);
            fprintf(stderr, "open(%s) error - %s\n", pidfile, strerror(errno));
            break;
        }

        snprintf(str, sizeof(str), "%d", getpid());
        write(fd, str, strlen(str));
        close(fd);

        ret = 0;
    } while (0);

    return ret;
}

#ifdef UT_TEST
int main(int argc, char *argv[])
{
    int ret = lock_write_pid("x.lock", "x.pid");
    if (ret != 0) {
        printf("Another process is running - %s\n", strerror(errno));
        return ret;
    }
    printf("ok\n");
    while(1) { sleep(1); }
    return 0;
}
#endif
