#define LOG_TAG "wifictrl"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "hostapd_config.h"
#include "dnsmasq_config.h"

typedef enum {
    AP_START = 1000,
    AP_SSID,
    AP_BRIDGE,
    AP_IFACE,
    AP_SEC,
    AP_WPAVER,
    AP_PSK,
    AP_NAS_IP,
    AP_NAS_ID,
    AP_AUTH_SRV_ADDR,
    AP_AUTH_SRV_PORT,
    AP_AUTH_SRV_KEY,
    AP_ACCT_SRV_ADDR,
    AP_ACCT_SRV_PORT,
    AP_ACCT_SRV_KEY,
    AP_BAND,
    AP_CH,
    AP_11N,
    AP_11AC,
    AP_BANDWIDTH,
    AP_COUNTRY,
    AP_HIDDEN,
    AP_LOAD_PATH,
    DNS_START,
    HOST_IP,
    NETMASK,
    DHCP_START_IP,
    DHCP_END_IP,
    DHCP_DNS_IP,
    DNSMASQ_LOAD_PATH
} PARAMS;

int main(int argc, char *argv[])
{
    struct hostapd_config *ap_config;
    struct dnsmasq_config *dhcpd_config;
    int ret;

    bool ap_start = false;
    bool dnsmasq_start = false;
    char *ap_load_path = NULL;
    char *dnsmasq_load_path = NULL;

    ap_config = hostapd_config_alloc();
    dhcpd_config = dnsmasq_config_alloc();

    if (ret < 0) {
        printf("hostapd_config init failed\n");
        exit(1);
    }

    for (;;) {
        int option_index = 0;
        static struct option long_options[] = {
          { "ap-start", no_argument, 0, AP_START },
          { "ap-ssid", required_argument, 0, AP_SSID },
          { "ap-bridge", required_argument, 0, AP_BRIDGE },
          { "ap-interface", required_argument, 0, AP_IFACE },
          { "ap-security", required_argument, 0, AP_SEC},
          { "ap-wpa-version", required_argument, 0, AP_WPAVER },
          { "ap-psk", required_argument, 0, AP_PSK },
          { "ap-nas-ip", required_argument, 0, AP_NAS_IP },
          { "ap-nas-identifier", required_argument, 0 , AP_NAS_ID },
          { "ap-auth-svr-addr", required_argument, 0, AP_AUTH_SRV_ADDR },
          { "ap-auth-svr-port", required_argument, 0, AP_AUTH_SRV_PORT },
          { "ap-auth-svr-key", required_argument, 0, AP_AUTH_SRV_KEY },
          { "ap-acct-svr-addr", required_argument, 0, AP_ACCT_SRV_ADDR },
          { "ap-acct-svr-port", required_argument, 0, AP_ACCT_SRV_PORT },
          { "ap-acct-svr-key", required_argument, 0, AP_ACCT_SRV_KEY },
          { "ap-band", required_argument, 0, AP_BAND },
          { "ap-channel", required_argument, 0, AP_CH },
          { "ap-11n", required_argument, 0, AP_11N },
          { "ap-11ac", required_argument, 0, AP_11AC },
          { "ap-bandwidth", required_argument, 0, AP_BANDWIDTH },
          { "ap-country", required_argument, 0, AP_COUNTRY },
          { "ap-hidden", required_argument, 0, AP_HIDDEN },
          { "ap-load-path", required_argument, 0, AP_LOAD_PATH },
          { "dnsmasq-start", no_argument, 0, DNS_START },
          { "host-ip", required_argument, 0, HOST_IP },
          { "netmask", required_argument, 0, NETMASK },
          { "dhcp-start-ip", required_argument, 0, DHCP_START_IP },
          { "dhcp-end-ip", required_argument, 0, DHCP_END_IP },
          { "dhcp-dns-ip", required_argument, 0, DHCP_DNS_IP },
          { "dnsmasq-load-path", required_argument, 0, DNSMASQ_LOAD_PATH },
          { 0, 0, 0, 0 }
        };

        ret = getopt_long(argc, argv, "", long_options, &option_index);

	if (ret < 0)
            break;

        switch (ret) {
            case AP_START:
                ap_start = true;
                break;
            case AP_SSID:
                ap_config->ssid = strdup(optarg);
                break;
            case AP_BRIDGE:
                ap_config->bridge = strdup(optarg);
                break;
            case AP_IFACE:
                ap_config->interface = strdup(optarg);
                dhcpd_config->interface = strdup(optarg);
                break;
            case AP_SEC:
                ap_config->security = atoi(optarg);
                break;
            case AP_WPAVER:
                ap_config->wpa_version = atoi(optarg);
                break;
            case AP_PSK:
                ap_config->psk = strdup(optarg);
                break;
            case AP_NAS_IP:
                ap_config->own_ip_addr = strdup(optarg);
                break;
            case AP_NAS_ID:
                ap_config->nas_identifier = strdup(optarg);
                break;
            case AP_AUTH_SRV_ADDR:
                ap_config->auth_svr_addr = strdup(optarg);
                break;
            case AP_AUTH_SRV_PORT:
                ap_config->auth_svr_port = atoi(optarg);
                break;
            case AP_AUTH_SRV_KEY:
                ap_config->auth_svr_key = strdup(optarg);
                break;
            case AP_ACCT_SRV_ADDR:
                ap_config->acct_svr_addr = strdup(optarg);
                break;
            case AP_ACCT_SRV_PORT:
                ap_config->acct_svr_port = atoi(optarg);
                break;
            case AP_ACCT_SRV_KEY:
                ap_config->acct_svr_key = strdup(optarg);
                break;
            case AP_BAND:
                ap_config->band = atoi(optarg);
                break;
            case AP_CH:
                ap_config->channel = atoi(optarg);
                break;
            case AP_11N:
                ap_config->moden = atoi(optarg);
                break;
            case AP_11AC:
                ap_config->modeac = atoi(optarg);
                break;
            case AP_BANDWIDTH:
                ap_config->bandwidth = atoi(optarg);
                break;
            case AP_COUNTRY:
                ap_config->country = strdup(optarg);
                break;
            case AP_HIDDEN:
                ap_config->hidden = atoi(optarg);
                break;
            case AP_LOAD_PATH:
                ap_load_path = strdup(optarg);
                break;
            case DNS_START:
                dnsmasq_start = true;
                break;
            case HOST_IP:
                dhcpd_config->host_ip = strdup(optarg);
                break;
            case NETMASK:
                dhcpd_config->netmask = strdup(optarg);
                break;
            case DHCP_START_IP:
                dhcpd_config->start_ip = strdup(optarg);
                break;
            case DHCP_END_IP:
                dhcpd_config->end_ip = strdup(optarg);
                break;
            case DHCP_DNS_IP:
                dhcpd_config->dns_ip = strdup(optarg);
                break;
            case DNSMASQ_LOAD_PATH:
                dnsmasq_load_path = strdup(optarg);
                break;
            default:
                goto out;
        }
    }

    if (ap_load_path) {
        ret = hostapd_config_read(ap_load_path, ap_config);
        if (ret < 0) {
            printf("%s read failed\n", ap_load_path);
            goto out;
        }
    }

    ret = hostapd_config_validate(ap_config);
    if (ret < 0) {
        printf("can't create validate hostapd config\n");
        goto out;
    }

    if (dnsmasq_load_path) {
        ret = dnsmasq_config_read(dnsmasq_load_path, dhcpd_config);
        if (ret < 0) {
            printf("%s read failed\n", dnsmasq_load_path);
            goto out;
        }
    }

    ret = dnsmasq_config_validate(dhcpd_config);
    if (ret < 0) {
        printf("can't create validate dnsmasq config\n");
        goto out;
    }

    hostapd_config_dump(ap_config);
    dnsmasq_config_dump(dhcpd_config);

out:
    hostapd_config_free(ap_config);
    dnsmasq_config_free(dhcpd_config);
    if (ap_load_path)
        free(ap_load_path);
    if (dnsmasq_load_path)
        free(dnsmasq_load_path);

    return 0;
}
