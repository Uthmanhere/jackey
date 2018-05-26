
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "freertos/queue.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <math.h>



#ifndef MPU6050_H_
#define MPU6050_H_

struct dim 
{
	uint16_t x;
	uint16_t y;
	uint16_t z;
};

struct mpu6050_readings
{
	struct dim accel;
	uint16_t temperature;
	struct dim gyro;
	struct dim angle
};

xQueueHandle xQueueGyro;

void mpu6050_read_task(void *);

#endif
