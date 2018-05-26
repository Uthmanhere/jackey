# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/azure-iot-pal-esp32/pal $(IDF_PATH)/components/azure-iot-pal-esp32/pal/inc $(IDF_PATH)/components/azure-iot-pal-esp32/sdk/c-utility/inc $(IDF_PATH)/components/azure-iot-pal-esp32/sdk/c-utility/inc/azure_c_shared_utility $(IDF_PATH)/components/azure-iot-pal-esp32/sdk/c-utility/pal/inc $(IDF_PATH)/components/azure-iot-pal-esp32/sdk/c-utility/pal/freertos $(IDF_PATH)/components/azure-iot-pal-esp32/sdk/c-utility/pal/generic $(IDF_PATH)/components/azure-iot-pal-esp32/sdk/iothub_client/inc $(IDF_PATH)/components/azure-iot-pal-esp32/sdk/serializer/inc $(IDF_PATH)/components/azure-iot-pal-esp32/sdk/umqtt/inc $(IDF_PATH)/components/azure-iot-pal-esp32/sdk/umqtt/inc/azure_umqtt_c $(IDF_PATH)/components/azure-iot-pal-esp32/sdk/deps/parson
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/azure-iot-pal-esp32 -lazure-iot-pal-esp32
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += azure-iot-pal-esp32
component-azure-iot-pal-esp32-build: 
