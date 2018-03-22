#define LOG_TAG "wifictrl"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "hostapd_config.h"
#include "dnsmasq_config.h"

int main(int argc, char *argv[])
{
    struct hostapd_config *ap_config;
    struct dnsmasq_config *dhcpd_config;
    int ret;

    bool ap_start = false;
    bool cs_read = false;
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
          { "ap-start", no_argument, 0, 1000 },
          { "ap-ssid", required_argument, 0, 1001 },
          { "ap-bridge", required_argument, 0, 1002 },
          { "ap-interface", required_argument, 0, 1003 },
          { "ap-security", required_argument, 0, 1004},
          { "ap-wpa-version", required_argument, 0, 1005 },
          { "ap-psk", required_argument, 0, 1006 },
          { "ap-nas-ip", required_argument, 0, 1007 },
          { "ap-nas-identifier", required_argument, 0 , 1008 },
          { "ap-auth-svr-addr", required_argument, 0, 1009 },
          { "ap-auth-svr-port", required_argument, 0, 1010 },
          { "ap-auth-svr-key", required_argument, 0, 1011 },
          { "ap-acct-svr-addr", required_argument, 0, 1012 },
          { "ap-acct-svr-port", required_argument, 0, 1013 },
          { "ap-acct-svr-key", required_argument, 0, 1014 },
          { "ap-band", required_argument, 0, 1015 },
          { "ap-channel", required_argument, 0, 1016 },
          { "ap-11n", required_argument, 0, 1017 },
          { "ap-11ac", required_argument, 0, 1018 },
          { "ap-bandwidth", required_argument, 0, 1019 },
          { "ap-country", required_argument, 0, 1020 },
          { "ap-hidden", required_argument, 0, 1021 },
          { "ap-load-path", required_argument, 0, 1022 },
          { "cs-read", no_argument, 0, 1023 },
          { "dnsmasq-start", no_argument, 0, 1024 },
          { "host-ip", required_argument, 0, 1025 },
          { "netmask", required_argument, 0, 1026 },
          { "dhcp-start-ip", required_argument, 0, 1027 },
          { "dhcp-end-ip", required_argument, 0, 1028 },
          { "dns-ip", required_argument, 0, 1029 },
          { "dnsmasq-load-path", required_argument, 0, 1030 },
          { 0, 0, 0, 0 }
        };

        ret = getopt_long(argc, argv, "", long_options, &option_index);

	if (ret < 0)
            break;

        switch (ret) {
            case 1000:
                ap_start = true;
                break;
            case 1001:
                ap_config->ssid = strdup(optarg);
                break;
            case 1002:
                ap_config->bridge = strdup(optarg);
                break;
            case 1003:
                ap_config->interface = strdup(optarg);
                break;
            case 1004:
                ap_config->security = atoi(optarg);
                break;
            case 1005:
                ap_config->wpa_version = atoi(optarg);
                break;
            case 1006:
                ap_config->psk = strdup(optarg);
                break;
            case 1007:
                ap_config->own_ip_addr = strdup(optarg);
                break;
            case 1008:
                ap_config->nas_identifier = strdup(optarg);
                break;
            case 1009:
                ap_config->auth_svr_addr = strdup(optarg);
                break;
            case 1010:
                ap_config->auth_svr_port = atoi(optarg);
                break;
            case 1011:
                ap_config->auth_svr_key = strdup(optarg);
                break;
            case 1012:
                ap_config->acct_svr_addr = strdup(optarg);
                break;
            case 1013:
                ap_config->acct_svr_port = atoi(optarg);
                break;
            case 1014:
                ap_config->acct_svr_key = strdup(optarg);
                break;
            case 1015:
                ap_config->band = atoi(optarg);
                break;
            case 1016:
                ap_config->channel = atoi(optarg);
                break;
            case 1017:
                ap_config->moden = atoi(optarg);
                break;
            case 1018:
                ap_config->modeac = atoi(optarg);
                break;
            case 1019:
                ap_config->bandwidth = atoi(optarg);
                break;
            case 1020:
                ap_config->country = strdup(optarg);
                break;
            case 1021:
                ap_config->hidden = atoi(optarg);
                break;
            case 1022:
                ap_load_path = strdup(optarg);
                break;
            case 1023:
                cs_read = true;
                break;
            case 1024:
                dnsmasq_start = true;
                break;
            case 1025:
                dhcpd_config->host_ip = strdup(optarg);
                break;
            case 1026:
                dhcpd_config->netmask = strdup(optarg);
                break;
            case 1027:
                dhcpd_config->start_ip = strdup(optarg);
                break;
            case 1028:
                dhcpd_config->end_ip = strdup(optarg);
                break;
            case 1029:
                dhcpd_config->dns_ip = strdup(optarg);
                break;
            case 1030:
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

    if (dnsmasq_load_path) {
        ret = dnsmasq_config_read(dnsmasq_load_path, dhcpd_config);
        if (ret < 0) {
            printf("%s read failed\n", dnsmasq_load_path);
            goto out;
        }
    }

    if (cs_read) {
        // Need to implement
        printf("read config from centralstore\n");
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
