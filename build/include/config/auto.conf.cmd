deps_config := \
	/home/usman/edu/jack/esp32/esp-idf/components/app_trace/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/aws_iot/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/bt/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/driver/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/esp32/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/esp_adc_cal/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/ethernet/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/fatfs/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/freertos/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/heap/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/libsodium/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/log/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/lwip/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/mbedtls/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/openssl/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/pthread/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/spi_flash/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/spiffs/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/tcpip_adapter/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/wear_levelling/Kconfig \
	/home/usman/edu/jack/esp32/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/usman/edu/jack/esp32/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/usman/edu/jack/esp32/esp-idf/components/azure-iot-pal-esp32/sample/main/Kconfig.projbuild \
	/home/usman/edu/jack/esp32/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/usman/edu/jack/esp32/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
