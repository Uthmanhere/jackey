// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h" 
#include "esp_log.h"

//#include "mpu6050.h"
#include "ad8232.h"
#include "lm35.h"
#include "nvs_flash.h"
#include "mqtt.h"
#include "azure.h"



void app_main()
{
	
	xTaskCreate(azure_task, "azure_task", 5120, NULL, 5, NULL);
    xTaskCreate(mqtt_task, "mqtt_task", 5120, NULL, 5, NULL);
}
