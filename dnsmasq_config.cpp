#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dnsmasq_config.h"

int dnsmasq_config_read(char *file_path, struct dnsmasq_config *config)
{
    FILE *fp;
    char line[256];
    char key[256];
    char val[256];
    char *cptr;
    char *lptr;
    char *vptr;

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
            vptr = val;
            if (strcmp(key, "interface") == 0 && !config->interface)
                config->interface = strdup(vptr);
            if (strcmp(key, "listen-address") == 0 && !config->host_ip)
                config->host_ip = strdup(vptr);
            if (strcmp(key, "server") == 0 && !config->dns_ip)
                config->dns_ip = strdup(vptr);
            if (strcmp(key, "dhcp-range") == 0) {
                cptr = strchr(vptr, ',');
                if (cptr)
                   *cptr = 0;
                if (!config->start_ip)
                    config->start_ip = strdup(vptr);
                vptr = cptr + 1;
                cptr = strchr(vptr, ',');
                if (cptr)
                   *cptr = 0;
                if (!config->end_ip)
                    config->end_ip = strdup(vptr);
                vptr = cptr + 1;
                cptr = strchr(vptr, ',');
                if (cptr)
                   *cptr = 0;
                else
                   continue;

                if (!config->netmask)
                    config->netmask = strdup(vptr);
            }
        }
    }

    fclose(fp);
    return 0;
}

struct dnsmasq_config *dnsmasq_config_alloc()
{
    struct dnsmasq_config *config;
    config = (struct dnsmasq_config *)malloc(sizeof(struct dnsmasq_config));

    config->interface = NULL;
    config->host_ip = NULL;
    config->netmask = NULL;
    config->start_ip = NULL;
    config->end_ip = NULL;
    config->dns_ip = NULL;

    return config;
}

void dnsmasq_config_free(struct dnsmasq_config *config)
{
    if (config) {
        if (config->interface)
            free(config->interface);
        if (config->host_ip)
            free(config->host_ip);
        if (config->netmask)
            free(config->netmask);
        if (config->start_ip)
            free(config->start_ip);
        if (config->end_ip)
            free(config->end_ip);
        if (config->dns_ip)
            free(config->dns_ip);

        free(config);
    }
}

void dnsmasq_config_dump(struct dnsmasq_config *config)
{
    if (config) {
        if (config->interface)
            printf("interface=%s\n", config->interface);
        if (config->host_ip)
            printf("host_ip=%s\n", config->host_ip);
        if (config->netmask)
            printf("netmask=%s\n", config->netmask);
        if (config->start_ip)
            printf("start_ip=%s\n", config->start_ip);
        if (config->end_ip)
            printf("end_ip=%s\n", config->end_ip);
        if (config->dns_ip)
            printf("dns_ip=%s\n", config->dns_ip);
    }
}

int dnsmasq_config_validate(struct dnsmasq_config *config)
{
    if (!config->interface)
        config->interface = strdup("wlan0");
    if (!config->host_ip)
        config->host_ip = strdup("192.168.100.10");
    if (!config->netmask)
        config->netmask = strdup("255.255.255.0");
    if (!config->start_ip)
        config->start_ip = strdup("192.168.100.11");
    if (!config->end_ip)
        config->end_ip = strdup("192.168.100.100");
    if (!config->dns_ip)
        config->dns_ip = strdup("192.168.100.10");

    return 0;
}

int dnsmasq_config_create(char *file_path, struct dnsmasq_config *config)
{
    FILE *fp;

    fp = fopen(file_path, "w");
    if (!fp)
        return -1;

    fprintf(fp, "interface=%s\n", config->interface);
    fprintf(fp, "listen-address=%s\n", config->host_ip);
    fprintf(fp, "bind-interfaces\n");
    fprintf(fp, "server=%s\n", config->dns_ip);
    fprintf(fp, "dhcp-range=%s,%s,%s,12h\n", config->start_ip,
            config->end_ip, config->netmask);

    fclose(fp);

    return 0;
}
