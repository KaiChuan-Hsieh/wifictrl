all:
	$(CC) main.cpp hostapd_config.cpp dnsmasq_config.cpp -o wifictrl
clean:
	rm wifictrl