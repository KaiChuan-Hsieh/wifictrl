#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>

int get_interface_state(const char *ifname)
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
