#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hostapd_config.h"

struct ht40_sideband_map {
    int channel;
    char sideband;
};

struct ht40_sideband_map ht40_sideband_maps[] = {
    { 1, '+' },
    { 2, '+' },
    { 3, '+' },
    { 4, '+' },
    { 5, '+' },
    { 6, '+' },
    { 7, '+' },
    { 8, '-' },
    { 9, '-' },
    { 10, '-' },
    { 11, '-' },
    { 12, '-' },
    { 13, '-' },
    { 36, '+' },
    { 40, '-' },
    { 44, '+' },
    { 48, '-' },
    { 52, '+' },
    { 56, '-' },
    { 60, '+' },
    { 64, '-' },
    { 100, '+' },
    { 104, '-' },
    { 108, '+' },
    { 112, '-' },
    { 116, '+' },
    { 120, '-' },
    { 124, '+' },
    { 128, '-' },
    { 132, '+' },
    { 136, '-' },
    { 140, '+' },
    { 144, '-' },
    { 149, '+' },
    { 153, '-' },
    { 157, '+' },
    { 161, '-' }
};

int hostapd_config_read(char *file_path, struct hostapd_config *config)
{
    FILE *fp;
    char line[256];
    char buf[256];
    char val[256];
    char *cptr;
    char *lptr;

    fp = fopen(file_path, "r");
    if (!fp)
        return -1;

    while (fgets(line, 256, fp)) {
        memset(buf, 0, 256);
        memset(val, 0, 256);

        for (lptr = line; *lptr == ' '; lptr++)
            ;
        if (*lptr == '#' || *lptr == '\n' || *lptr == '\0')
            continue;

        cptr = strchr(lptr, '=');
        if (cptr) {
            strncpy(buf, lptr, cptr - lptr);
            strncpy(val, cptr + 1, strlen(lptr) - (cptr - lptr + 1));
            cptr = strpbrk(val, "\n# ");
            if (cptr)
                *cptr = 0;

            if (strcmp(buf, "ssid") == 0 && !config->ssid)
                config->ssid = strdup(val);
            if (strcmp(buf, "bridge") == 0 && !config->bridge)
                config->bridge = strdup(val);
            if (strcmp(buf, "interface") == 0 && !config->interface)
                config->interface = strdup(val);
            if (strcmp(buf, "wpa_key_mgmt") == 0 && config->security == -1) {
                if (strcmp(val, "WPA-PSK") == 0)
                    config->security = 1;
                else if (strcmp(val, "WPA-EAP") == 0)
                    config->security = 2;
                else
                    config->security = 0;
            }
            if (strcmp(buf, "wpa") == 0 && config->wpa_version == -1)
                config->wpa_version = atoi(val);
            if (strcmp(buf, "wpa_passphrase") == 0 && !config->psk)
                config->psk = strdup(val);
            if (strcmp(buf, "own_ip_addr") == 0 && !config->own_ip_addr)
                config->own_ip_addr = strdup(val);
            if (strcmp(buf, "nas_identifier") == 0 && !config->nas_identifier)
                config->nas_identifier = strdup(val);
            if (strcmp(buf, "auth_server_addr") == 0 && !config->auth_svr_addr)
                config->auth_svr_addr = strdup(val);
            if (strcmp(buf, "auth_server_port") == 0 && config->auth_svr_port == -1)
                config->auth_svr_port = atoi(val);
            if (strcmp(buf, "auth_server_shared_secret") == 0 && !config->auth_svr_key)
                config->auth_svr_key = strdup(val);
            if (strcmp(buf, "acct_server_addr") == 0 && !config->acct_svr_addr)
                config->acct_svr_addr = strdup(val);
            if (strcmp(buf, "acct_server_port") == 0 && config->acct_svr_port == -1)
                config->acct_svr_port = atoi(val);
            if (strcmp(buf, "acct_server_shared_secret") == 0 && !config->acct_svr_key)
                config->acct_svr_key = strdup(val);
            if (strcmp(buf, "hw_mode") == 0 && config->band == ILLEGAL)
                config->band = (strcmp(val, "a") == 0? BAND_5G: BAND_24G);
            if (strcmp(buf, "channel") == 0 && config->channel == -1)
                config->channel = atoi(val);
            if (strcmp(buf, "ieee80211n") == 0 && config->moden == -1)
                config->moden = atoi(val);
            if (strcmp(buf, "ieee80211ac") == 0 && config->modeac == -1)
                config->modeac = atoi(val);
            if (strcmp(buf, "country") == 0 && !config->country)
                config->country = strdup(val);
            if (strcmp(buf, "ignore_broadcast_ssid") == 0 && config->hidden == -1)
                config->hidden = atoi(val);
            if (strcmp(buf, "vht_oper_chwidth") == 0 && config->bandwidth == ILLEGAL)
                config->bandwidth = atoi(val);
            if (strcmp(buf, "ht_capab") == 0 && config->bandwidth == ILLEGAL) {
                if (strstr(val, "40"))
                    config->bandwidth = HT40;
            }
        }
    }

    fclose(fp);

    return 0;
}

void hostapd_config_dump(struct hostapd_config *config)
{
    if (config) {
        if (config->ssid)
            printf("ssid=%s\n", config->ssid);
        if (config->bridge)
            printf("bridge=%s\n", config->bridge);
        if (config->interface)
            printf("interface=%s\n", config->interface);
        if (config->security != -1)
            printf("security=%d\n", config->security);
        if (config->wpa_version != -1)
            printf("wpa=%d\n", config->wpa_version);
        if (config->psk)
            printf("psk=%s\n", config->psk);
        if (config->own_ip_addr)
            printf("own_ip_addr=%s\n", config->own_ip_addr);
        if (config->nas_identifier)
            printf("nas_identifier=%s\n", config->nas_identifier);
        if (config->auth_svr_addr)
            printf("auth_svr_addr=%s\n", config->auth_svr_addr);
        if (config->auth_svr_port != -1)
            printf("auth_svr_port=%d\n", config->auth_svr_port);
        if (config->auth_svr_key)
            printf("auth_svr_key=%s\n", config->auth_svr_key);
        if (config->acct_svr_addr)
            printf("acct_svr_addr=%s\n", config->acct_svr_addr);
        if (config->acct_svr_port != -1)
            printf("acct_svr_port=%d\n", config->acct_svr_port);
        if (config->acct_svr_key)
            printf("acct_svr_key=%s\n", config->acct_svr_key);
        if (config->band != ILLEGAL)
            printf("band=%d\n", config->band);
        if (config->channel != -1)
            printf("channel=%d\n", config->channel);
        if (config->moden != -1)
            printf("moden=%d\n", config->moden);
        if (config->modeac != -1)
            printf("modeac=%d\n", config->modeac);
        if (config->bandwidth != ILLEGAL)
            printf("bandwidth=%d\n", config->bandwidth);
        if (config->country)
            printf("country=%s\n", config->country);
        if (config->hidden != -1)
            printf("hidden=%d\n", config->hidden);
    }
}

struct hostapd_config* hostapd_config_alloc()
{
    struct hostapd_config *config;
    config = (struct hostapd_config *)
        malloc(sizeof(struct hostapd_config));

    if (config == NULL) 
        return NULL;

    config->ssid = NULL;
    config->bridge = NULL;
    config->interface = NULL;
    config->security = -1;
    config->wpa_version = -1;
    config->psk = NULL;
    config->own_ip_addr = NULL;
    config->nas_identifier = NULL;
    config->auth_svr_addr = NULL;
    config->auth_svr_port = -1;
    config->auth_svr_key = NULL;
    config->acct_svr_addr = NULL;
    config->acct_svr_port = -1;
    config->acct_svr_key = NULL;
    config->band = ILLEGAL;
    config->channel = -1;
    config->moden = -1;
    config->modeac = -1;
    config->bandwidth = ILLEGAL;
    config->country = NULL;
    config->hidden = -1;

    return config;
}

void hostapd_config_free(struct hostapd_config *config)
{
    if (config) {
        if (config->ssid)
            free(config->ssid);
        if (config->bridge)
            free(config->bridge);
        if (config->interface)
            free(config->interface);
        if (config->security != -1)
            config->security = -1;
        if (config->wpa_version != -1)
            config->wpa_version = -1;
        if (config->psk)
            free(config->psk);
        if (config->own_ip_addr)
            free(config->own_ip_addr);
        if (config->nas_identifier)
            free(config->nas_identifier);
        if (config->auth_svr_addr)
            free(config->auth_svr_addr);
        if (config->auth_svr_port != -1)
            config->auth_svr_port = -1;
        if (config->auth_svr_key)
            free(config->auth_svr_key);
        if (config->acct_svr_addr)
            free(config->acct_svr_addr);
        if (config->acct_svr_port != -1)
            config->acct_svr_port = -1;
        if (config->acct_svr_key)
            free(config->acct_svr_key);
        if (config->band != ILLEGAL)
            config->band = ILLEGAL;
        if (config->channel != -1)
            config->channel = -1;
        if (config->moden != -1)
            config->moden = -1;
        if (config->modeac != -1)
            config->modeac = -1;
        if (config->bandwidth != ILLEGAL)
            config->bandwidth = ILLEGAL;
        if (config->hidden != -1)
            config->hidden = -1;
        if (config->country)
            free(config->country);

        free(config);
    }
}

int hostapd_config_validate(struct hostapd_config *config)
{
    /* The ssid must be assigned */
    if (!config->ssid)
        config->ssid = strdup("default_ssid");

    /* The interface must be assigned */
    if (!config->interface)
        config->interface = strdup("wlan0");

    /* One of the security must be set, 0: NONE, 1: WPA-PSK, 2: WPA-EAP */
    if (config->security == -1) {
        config->security = 0;
    } else if (config->security == 1) {
        if (config->wpa_version < 1)
            config->wpa_version = 3;
        if (!config->psk)
            config->psk = strdup("default_password");
        if (config->own_ip_addr) {
            free(config->own_ip_addr);
            config->own_ip_addr = NULL;
        }
        if (config->nas_identifier) {
            free(config->nas_identifier);
            config->nas_identifier = NULL;
        }
        if (config->auth_svr_addr) {
            free(config->auth_svr_addr);
            config->auth_svr_addr = NULL;
        }
        if (config->auth_svr_port != -1)
            config->auth_svr_port = -1;
        if (config->auth_svr_key) {
            free(config->auth_svr_key);
            config->auth_svr_key = NULL;
        }
        if (config->acct_svr_addr) {
            free(config->acct_svr_addr);
            config->acct_svr_addr = NULL;
        }
        if (config->acct_svr_port != -1)
            config->acct_svr_port = -1;
        if (config->acct_svr_key) {
            free(config->acct_svr_key);
            config->acct_svr_key = NULL;
        }
    } else if (config->security == 2) {
        if (config->wpa_version < 1)
            config->wpa_version = 3;
        if (!config->own_ip_addr && !config->nas_identifier)
            config->own_ip_addr = strdup("127.0.0.1");
        if (!config->auth_svr_addr)
            config->auth_svr_addr = strdup("127.0.0.1");
        if (config->auth_svr_port == -1)
            config->auth_svr_port = 1812;
        if (!config->auth_svr_key)
            config->auth_svr_key = strdup("secret");
        if (!config->acct_svr_addr)
            config->acct_svr_addr = strdup("127.0.0.1");
        if (config->acct_svr_port == -1)
            config->acct_svr_port = 1813;
        if (!config->acct_svr_key)
            config->acct_svr_key = strdup("secret");
        if (config->psk) {
            free(config->psk);
            config->psk = NULL;
        }
    }

    if (!config->country)
        config->country = strdup("US");

    /* Country code WW can only support 2.4GHz */
    if (strcmp(config->country, "WW") == 0) {
        config->band = BAND_24G;
        if (config->channel > 13)
            config->channel = 0;
    }

    /* Validate country-channel mapping */
    if (config->band == ILLEGAL) {
        config->band = BAND_24G;
    } else if (config->band == BAND_24G) {
        if (config->channel > 13 && config->channel != 0)
            config->channel = 0;
    } else if (config->band == BAND_5G) {
        if (config->channel < 36 && config->channel != 0)
            config->channel = 0;
    }

    /* Bandwidth support, 0: 20MHz, 1: 40MHz, 2: 80MHz */
    if (config->moden == -1) {
        config->moden = 0;
        config->bandwidth = HT20;
    }

    if (config->modeac == -1) {
        config->modeac = 0;
    } else if (config->modeac == 1) {
        config->moden = 1;
    }

    if (config->bandwidth == ILLEGAL)
        config->bandwidth = HT20;

    if (config->moden == 1 && config->modeac == 0 && config->bandwidth > HT40)
        config->bandwidth = HT20;

    if (config->hidden == -1)
        config->hidden = 0;

    return 0;
}

int hostapd_config_create(char *file_path, struct hostapd_config *config)
{
    FILE *fp;
    int i;
    int n;

    fp = fopen(file_path, "w");
    if (!fp)
        return -1;

    fprintf(fp, "ctrl_interface=/var/run/hostapd\n");
    fprintf(fp, "interface=%s\n", config->interface);

    if (config->bridge)
        fprintf(fp, "bridge=%s\n", config->bridge);

    fprintf(fp, "ctrl_interface_group=0\n");
    fprintf(fp, "ssid=%s\n", config->ssid);
    fprintf(fp, "country_code=%s\n", config->country);
    fprintf(fp, "ieee80211d=1\nieee80211h=1\n");
    fprintf(fp, "local_pwr_constraint=3\n");
    fprintf(fp, "spectrum_mgmt_required=1\n");
    if (config->band == BAND_5G)
        fprintf(fp, "hw_mode=a\n");
    else
        fprintf(fp, "hw_mode=g\n");
    fprintf(fp, "channel=%d\n", config->channel);
    fprintf(fp, "preamble=1\n");
    fprintf(fp, "auth_algs=3\n");
    fprintf(fp, "ignore_broadcast_ssid=%d\n", config->hidden);
    fprintf(fp, "wmm_enabled=1\n");

    if (config->moden) {
        fprintf(fp, "ieee80211n=%d\n", config->moden);
        fprintf(fp, "ht_capab=");
        if (config->bandwidth) {
            n = sizeof(ht40_sideband_maps)/sizeof(ht40_sideband_maps[0]);
            for (i = 0; i < n; i++) {
                if (config->channel == ht40_sideband_maps[i].channel) {
                    fprintf(fp, "[HT40%c]", ht40_sideband_maps[i].sideband);
                    break;
                }
            }

            if (i == n)
                fprintf(fp, "[HT40+][HT40-]");

            fprintf(fp, "[SHORT-GI-20][SHORT-GI-40][TX-STBC]\n");
        } else
            fprintf(fp, "[SHORT-GI-20][TX-STBC]\n");
    }

    if (config->modeac) {
        fprintf(fp, "ieee80211ac=%d\n", config->modeac);

        if (config->bandwidth == HT20 || config->bandwidth == HT40) {
            fprintf(fp, "vht_capab=[RXLDPC][TX-STBC-2BY1][SU-BEAMFOEMEE]\n");
            fprintf(fp, "vht_oper_chwidth=0\n");
        } else {
            fprintf(fp, "vht_capab=[SHORT-GI-80][RXLDPC][TX-STBC-2BY1][SU-BEAMFOEMEE]\n");
            fprintf(fp, "vht_oper_chwidth=1\n");
        }
    }

    if (config->security == 1) {
        fprintf(fp, "wpa_key_mgmt=WPA-PSK\n");
        fprintf(fp, "wpa=%d\n", config->wpa_version);
        if (config->wpa_version == 2)
            fprintf(fp, "rsn_pairwise=CCMP\n");
        else
            fprintf(fp, "wpa_pairwise=CCMP TKIP\n");
        fprintf(fp, "wpa_passphrase=%s\n", config->psk);
    } else if (config->security == 2) {
        fprintf(fp, "wpa_key_mgmt=WPA-EAP\n");
        fprintf(fp, "wpa=%d\n", config->wpa_version);
        if (config->wpa_version == 2)
            fprintf(fp, "rsn_pairwise=CCMP\n");
        else
            fprintf(fp, "wpa_pairwise=CCMP TKIP\n");
        if (config->own_ip_addr)
            fprintf(fp, "own_ip_addr=%s\n", config->own_ip_addr);
        if (config->nas_identifier)
            fprintf(fp, "nas_identifier=%s\n", config->nas_identifier);
        fprintf(fp, "auth_server_addr=%s\n", config->auth_svr_addr);
        fprintf(fp, "auth_server_port=%d\n", config->auth_svr_port);
        fprintf(fp, "auth_server_shared_secret=%s\n", config->auth_svr_key);
        fprintf(fp, "acct_server_addr=%s\n", config->acct_svr_addr);
        fprintf(fp, "acct_server_port=%d\n", config->acct_svr_port);
        fprintf(fp, "acct_server_shared_secret=%s\n", config->acct_svr_key);
    }

    fprintf(fp, "wowlan_triggers=any\n");

    fclose(fp);

    return 0;
}
