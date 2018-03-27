#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/types.h>

#include "dnsmasq_config.h"

#define DEFAULT_PID_PATH "/var/run/dnsmasq/dnsmasq.pid"

extern char *get_config_value(const char *file_path, const char *key);
extern int get_intf_state(const char *ifname);
extern int clear_intf_ipv4_address(const char *ifname);
extern int set_intf_ipv4_address(const char *ifname, const char *ipaddr);
extern int set_intf_ipv4_netmask(const char *ifname, const char *netmask);
extern pid_t get_pid(const char *file_path);
extern void free_params(char *argv[]);

int dnsmasq_ctrl_start(const char *dnsmasq_path, const char *config_path)
{
    pid_t pid;
    char *pid_path = strdup(DEFAULT_PID_PATH);
    char *argv[6] = {0};
    int ret = 0;
    int status;
    char *ifname;
    char *hostip;
    char *netmask = NULL;
    char *cur;
    char *strp;
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
            ret = get_intf_state(ifname);
            if (ret == 1) {
                break;
            }
            count++;
            sleep(1);
        }
    } else {
        ret = -1;
        goto out;
    }

    hostip = get_config_value(config_path, "listen-address");
    if (!hostip) {
        ret = -1;
        goto out;
    }

    strp = get_config_value(config_path, "dhcp-range");
    if (!strp) {
        ret = -1;
        goto out;
    }

    cur = strtok(strp, ",");
    while (cur) {
        if (strstr(cur, "255")) {
            netmask = cur;
            break;
        }
        cur = strtok(NULL, ",");
    }

    ret = clear_intf_ipv4_address(ifname);
    if (ret < 0) {
        printf("clear %s ipv4 address failed\n", ifname);
        goto out;
    }
    ret = set_intf_ipv4_address(ifname, hostip);
    if (ret < 0) {
        printf("set ipv4 address %s failed\n", hostip);
        goto out;
    }
    ret = set_intf_ipv4_netmask(ifname, netmask);
    if (ret < 0) {
        printf("set netmask %s failed\n", netmask);
        goto out;
    }

    argv[0] = strdup(dnsmasq_path);
    argv[1] = strdup("-C");
    argv[2] = strdup(config_path);
    argv[3] = strdup("-x");
    argv[4] = strdup(pid_path);
    argv[5] = NULL;

    pid = fork();
    if (pid == 0) {
        if (execv(dnsmasq_path, argv)) {
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
    if (hostip)
        free(hostip);
    if (strp)
        free(strp);
    free_params(argv);
    printf("%s Out\n", __func__);
    return ret;
}
