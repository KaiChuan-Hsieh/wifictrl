#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/types.h>

#include "hostapd_config.h"

#define DEFAULT_PID_PATH "/var/run/hostapd/hostapd.pid"

extern char *get_config_value(const char *file_path, const char *key);
extern int get_interface_state(const char *ifname);
extern pid_t get_pid(const char *file_path);

void free_params(char *argv[])
{
    int i = 0;
    while (argv[i]) {
        free(argv[i]);
        i++;
    }
}

int hostapd_ctrl_start(const char *hostapd_path, const char *config_path)
{
    pid_t pid;
    char *pid_path = strdup(DEFAULT_PID_PATH);
    char *argv[6] = {0};
    int ret = 0;
    int status;
    char *ifname;
    int count = 0;

    printf("%s In\n", __func__);

    if ((pid = get_pid(pid_path)) != -1) {
        kill(pid, SIGTERM);
    }

    if (ifname = get_config_value(config_path, "interface")) {
        while (1) {
            if (count > 5) {
                ret = -1;
                goto out;
            }
            if (ret < 0) {
                goto out;
            }
            ret = get_interface_state(ifname);
            if (ret == 0) {
                break;
            }
            count++;
            sleep(1);
        }
    } else {
        ret = -1;
        goto out;
    }

    argv[0] = strdup(hostapd_path);
    argv[1] = strdup("-P");
    argv[2] = strdup(DEFAULT_PID_PATH);
    argv[3] = strdup("-B");
    argv[4] = strdup(config_path);
    argv[5] = NULL;

    pid = fork();
    if (pid == 0) {
        if (execv(hostapd_path, argv)) {
            printf("execv failed (%s)", strerror(errno));
            exit(1);
        }
    } else if (pid < 0) {
        printf("fork failed (%s)", strerror(errno));
        ret = -1;
    } else {
        wait(&status);
        if (WEXITSTATUS(status))
            ret = -1;
        else
            ret = 0;
    }

out:
    if (pid_path)
        free(pid_path);
    if (ifname)
        free(ifname);
    free_params(argv);
    printf("%s Out\n", __func__);
    return ret;
}
