all:
	$(CC) main.cpp hostapd_config.cpp hostapd_ctrl.cpp dnsmasq_config.cpp fileutils.cpp netutils.cpp -o wifictrl
clean:
	rm wifictrl
