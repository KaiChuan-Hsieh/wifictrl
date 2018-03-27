#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

int get_intf_state(const char *ifname)
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

int clear_intf_ipv4_address(const char* ifname)
{
    int ifc_sock;
    struct ifreq ifr;
    struct sockaddr_in sin;

    ifc_sock = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (ifc_sock < 0) {
        printf("socket() failed: %s\n", strerror(errno));
        return -1;
    }

    memset(&ifr, 0, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    sin.sin_family = AF_INET;
    sin.sin_port = 0;
    sin.sin_addr.s_addr = 0;

    memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));
    if(ioctl(ifc_sock, SIOCGIFFLAGS, &ifr) < 0)
        return -1;

    close(ifc_sock);
    return 0;
}

int set_intf_ipv4_address(const char *ifname, const char *ipaddr)
{
    int ifc_sock;
    struct ifreq ifr;
    struct sockaddr_in sin;

    ifc_sock = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (ifc_sock < 0) {
        printf("socket() failed: %s\n", strerror(errno));
        return -1;
    }

    memset(&ifr, 0, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    sin.sin_family = AF_INET;
    sin.sin_port = 0;
    inet_aton(ipaddr, &sin.sin_addr);

    memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));
    if(ioctl(ifc_sock, SIOCSIFADDR, &ifr) < 0)
        return -1;

    close(ifc_sock);
    return 0;
}

int set_intf_ipv4_netmask(const char *ifname, const char *netmask)
{
    int ifc_sock;
    struct ifreq ifr;
    struct sockaddr_in sin;

    ifc_sock = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (ifc_sock < 0) {
        printf("socket() failed: %s\n", strerror(errno));
        return -1;
    }

    memset(&ifr, 0, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    sin.sin_family = AF_INET;
    sin.sin_port = 0;
    if (netmask)
        inet_aton(netmask, &sin.sin_addr);
    else
        inet_aton("255.255.255.0", &sin.sin_addr);

    memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));
    if(ioctl(ifc_sock, SIOCSIFNETMASK, &ifr) < 0)
        return -1;

    close(ifc_sock);
    return 0;

}
