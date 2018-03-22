#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hostapd_config.h"

int hostapd_config_read(char *file_path, struct hostapd_config *config)
{
    FILE *fp;
    char line[256];
    char key[256];
    char val[256];
    char *cptr;
    char *lptr;

    fp = fopen(file_path, "r");
    if (!fp)
        return -1;

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

            if (strcmp(key, "ssid") == 0 && !config->ssid)
                config->ssid = strdup(val);
            if (strcmp(key, "bridge") == 0 && !config->bridge)
                config->bridge = strdup(val);
            if (strcmp(key, "interface") == 0 && !config->interface)
                config->interface = strdup(val);
            if (strcmp(key, "wpa_key_mgmt") == 0 && config->security == -1) {
                if (strcmp(val, "WPA-PSK") == 0)
                    config->security = 1;
                else if (strcmp(val, "WPA-EAP") == 0)
                    config->security = 2;
                else
                    config->security = 0;
            }
            if (strcmp(key, "wpa") == 0 && config->wpa_version == -1)
                config->wpa_version = atoi(val);
            if (strcmp(key, "wpa_passphrase") == 0 && !config->psk)
                config->psk = strdup(val);
            if (strcmp(key, "own_ip_addr") == 0 && !config->own_ip_addr)
                config->own_ip_addr = strdup(val);
            if (strcmp(key, "nas_identifier") == 0 && !config->nas_identifier)
                config->nas_identifier = strdup(val);
            if (strcmp(key, "auth_server_addr") == 0 && !config->auth_svr_addr)
                config->auth_svr_addr = strdup(val);
            if (strcmp(key, "auth_server_port") == 0 && config->auth_svr_port == -1)
                config->auth_svr_port = atoi(val);
            if (strcmp(key, "auth_server_shared_secret") == 0 && !config->auth_svr_key)
                config->auth_svr_key = strdup(val);
            if (strcmp(key, "acct_server_addr") == 0 && !config->acct_svr_addr)
                config->acct_svr_addr = strdup(val);
            if (strcmp(key, "acct_server_port") == 0 && config->acct_svr_port == -1)
                config->acct_svr_port = atoi(val);
            if (strcmp(key, "acct_server_shared_secret") == 0 && !config->acct_svr_key)
                config->acct_svr_key = strdup(val);
            if (strcmp(key, "hw_mode") == 0 && config->band == -1)
                config->band = (strcmp(val, "a") == 0? 1: 0);
            if (strcmp(key, "channel") == 0 && config->channel == -1)
                config->channel = atoi(val);
            if (strcmp(key, "ieee80211n") == 0 && config->moden == -1)
                config->moden = atoi(val);
            if (strcmp(key, "ieee80211ac") == 0 && config->modeac == -1)
                config->modeac = atoi(val);
            if (strcmp(key, "country") == 0 && !config->country)
                config->country = strdup(val);
            if (strcmp(key, "ignore_broadcast_ssid") == 0 && config->hidden == -1)
                config->hidden = atoi(val);
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
            printf("wpa_psk=%s\n", config->psk);
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
        if (config->band != -1)
            printf("band=%d\n", config->band);
        if (config->channel != -1)
            printf("channel=%d\n", config->channel);
        if (config->moden != -1)
            printf("moden=%d\n", config->moden);
        if (config->modeac != -1)
            printf("modeac=%d\n", config->modeac);
        if (config->bandwidth != -1)
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
    config->band = -1;
    config->channel = -1;
    config->moden = -1;
    config->modeac = -1;
    config->bandwidth = -1;
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
        if (config->band != -1)
            config->band = -1;
        if (config->channel != -1)
            config->channel = -1;
        if (config->moden != -1)
            config->moden = -1;
        if (config->modeac != -1)
            config->modeac = -1;
        if (config->bandwidth != -1)
            config->bandwidth = -1;
        if (config->hidden != -1)
            config->hidden = -1;
        if (config->country)
            free(config->country);

        free(config);
    }
}
