struct dnsmasq_config {
    char *interface;
    char *host_ip;
    char *netmask;
    char *start_ip;
    char *end_ip;
    char *dns_ip;
};

struct dnsmasq_config *dnsmasq_config_alloc();
void dnsmasq_config_free(struct dnsmasq_config *config);
void dnsmasq_config_dump(struct dnsmasq_config *config);
int dnsmasq_config_read(char *dnsmasq_config_path,
                        struct dnsmasq_config *config);
int dnsmasq_config_validate(struct dnsmasq_config *config);
int dnsmasq_config_create(char *file_path, struct dnsmasq_config *config);

int dnsmasq_ctrl_start(const char *dnsmasq_path, const char *config_path);
