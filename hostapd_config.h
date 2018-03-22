struct hostapd_config {
    char *ssid;
    char *bridge;
    char *interface;
    int security;
    int wpa_version;
    char *psk;
    char *own_ip_addr;
    char *nas_identifier;
    char *auth_svr_addr;
    int auth_svr_port;
    char *auth_svr_key;
    char *acct_svr_addr;
    int acct_svr_port;
    char *acct_svr_key;
    int band;
    int channel;
    int moden;
    int modeac;
    int bandwidth;
    char *country;
    int hidden;
};

void hostapd_config_dump(struct hostapd_config *config);
struct hostapd_config *hostapd_config_alloc();
void hostapd_config_free(struct hostapd_config *config);
int hostapd_config_read(char *file_path, struct hostapd_config *config);
int hostapd_config_validate(struct hostapd_config *config);
