#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <net/if.h>

#include "hostapd_config.h"

#define DEFAULT_PID_PATH "/var/run/hostapd/hostapd.pid"

char *get_interface(char *file_path)
{
    FILE *fp;
    char line[256];
    char key[256];
    char val[256];
    char *cptr;
    char *lptr;
    char *ifname = NULL;

    printf("%s In\n", __func__);

    fp = fopen(file_path, "r");

    if (!fp)
        return NULL;

    while (fgets(line, 256, fp)) {
        memset(key, 0, 256);
        memset(val, 0, 256);

        for (lptr = line; *lptr == ' '; lptr++)
            ;
        if (*lptr == '#' || *lptr == '\n' || *lptr == '\0')
            continue;

        cptr = strchr(lptr, '=');

        if (cptr) {
            strncpy(key, lptr, cptr - lptr);
            strncpy(val, cptr + 1, strlen(lptr) - (cptr - lptr + 1));

            cptr = strpbrk(val, "\n# ");
            if (cptr)
                *cptr = 0;

            if (strcmp(key, "interface") == 0) {
                ifname = strdup(val);
                break;
            } else
                continue;
        }
    }

    fclose(fp);

    printf("%s Out\n", __func__);

    return ifname;
}    

pid_t get_pid(const char *file_path)
{
    FILE *fp;
    char line[256];
    int i = 0;
    pid_t pid;

    fp = fopen(file_path, "r");
    if (!fp)
        return -1;

    while (fgets(line, 256, fp)) {
        for (i = 0; i < sizeof(line); i++) {
            if (!isdigit(line[i])) {
                line[i] = '\0';
                break;
            }
        }
    }

    fclose(fp);

    if (i > 0)
        pid = atoi(line);
    else
        pid = -1;

    return pid;
}

void free_params(char *argv[])
{
    int i = 0;
    while (argv[i]) {
        free(argv[i]);
        i++;
    }
}

int get_interface_state(char *ifname)
{
    int ifc_sock;
    struct ifreq ifr;

    ifc_sock = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (ifc_sock < 0) {
        printf("socket() failed: %s\n", strerror(errno));
        return -1;
    }

    memset(&ifr, 0, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    if(ioctl(ifc_sock, SIOCGIFFLAGS, &ifr) < 0)
        return -1;

    close(ifc_sock);
    return ifr.ifr_flags & IFF_UP;
}

int hostapd_ctrl_start(char *hostapd_path, char *config_path)
{
    pid_t pid;
    char *pid_path = strdup(DEFAULT_PID_PATH);
    char *argv[6];
    int ret = 0;
    int status;
    char *ifname;
    int count = 0;

    printf("%s In\n", __func__);

    if ((pid = get_pid(pid_path)) != -1) {
        kill(pid, SIGTERM);
    }

    if (ifname = get_interface(config_path)) {
        while (1) {
            if (count > 5) {
                free(ifname);
                return -1;
            }
            if (ret < 0) {
                free(ifname);
                return -1;
            }
            ret = get_interface_state(ifname);
            if (ret == 0) {
                free(ifname);
                break;
            }
            count++;
            sleep(1);
        }
    } else
        return -1;

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

    free(pid_path);
    free_params(argv);
    printf("%s Out\n", __func__);
    return ret;
}
