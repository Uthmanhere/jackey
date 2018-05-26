#include "mpu6050.h"


#define PIN_SDA 21
#define PIN_CLK 22
#define I2C_ADDRESS 0x68 // I2C address of MPU6050

#define MPU6050_ACCEL_XOUT_H 0x43 
#define MPU6050_PWR_MGMT_1   0x6B

/*
 * The following registers contain the primary data we are interested in
 * 0x3B MPU6050_ACCEL_XOUT_H
 * 0x3C MPU6050_ACCEL_XOUT_L
 * 0x3D MPU6050_ACCEL_YOUT_H
 * 0x3E MPU6050_ACCEL_YOUT_L
 * 0x3F MPU6050_ACCEL_ZOUT_H
 * 0x50 MPU6050_ACCEL_ZOUT_L
 * 0x41 MPU6050_TEMP_OUT_H
 * 0x42 MPU6050_TEMP_OUT_L
 * 0x43 MPU6050_GYRO_XOUT_H
 * 0x44 MPU6050_GYRO_XOUT_L
 * 0x45 MPU6050_GYRO_YOUT_H
 * 0x46 MPU6050_GYRO_YOUT_L
 * 0x47 MPU6050_GYRO_ZOUT_H
 * 0x48 MPU6050_GYswRO_ZOUT_L
 */


#undef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x)   do { esp_err_t rc = (x); if (rc != ESP_OK) { ESP_LOGE("err", "esp_err_t = %d", rc); assert(0 && #x);} } while(0);



void mpu6050_read_task(void *ignore) 
{
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = PIN_SDA;
	conf.scl_io_num = PIN_CLK;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = 100000;
	i2c_param_config(I2C_NUM_0, &conf);
	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);

	i2c_cmd_handle_t cmd;
//	vTaskDelay(200/portTICK_PERIOD_MS);
//	puts("Installed Drivers");
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1);
	i2c_master_write_byte(cmd, MPU6050_ACCEL_XOUT_H, 1);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
//	puts("Sent A");


	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1));
	i2c_master_write_byte(cmd, MPU6050_PWR_MGMT_1, 1);
	i2c_master_write_byte(cmd, 0, 1);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
//	puts("Sent B");

	uint8_t data[14];
	xQueueGyro = xQueueCreate(2, sizeof(struct mpu6050_readings));
	struct mpu6050_readings readings;
	while(1)
	{
		cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1);
		i2c_master_write_byte(cmd, MPU6050_ACCEL_XOUT_H, 1);
		i2c_master_stop(cmd);
		ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS));
		i2c_cmd_link_delete(cmd);

		cmd = i2c_cmd_link_create();
		ESP_ERROR_CHECK(i2c_master_start(cmd));
		ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_READ, 1));

		for (int count = 0; count < 13; ++count)
			i2c_master_read_byte(cmd, data + count, 0);
		i2c_master_read_byte(cmd, data + 13, 1);

		//i2c_master_read(cmd, data, sizeof(data), 1);
		ESP_ERROR_CHECK(i2c_master_stop(cmd));
		ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS));
		i2c_cmd_link_delete(cmd);

		
		readings.accel.x = (data[0] << 8) | data[1];
		readings.accel.y = (data[2] << 8) | data[3];
		readings.accel.z = (data[4] << 8) | data[5];


		readings.temperature = (data[6] << 8) | data[7];

		readings.gyro.x = (data[8] << 8) | data[9];
		readings.gyro.y  = (data[10] << 8) | data[11];
		readings.gyro.z  = (data[12] << 8) | data[13];
//		ESP_LOGD(tag, "accel_x: %d, accel_y: %d, accel_z: %d", accel_x, accel_y, accel_z);
//		printf("accel_x: %d, accel_y: %d, accel_z: %d\n", accel_x, accel_y, accel_z);
		readings.angle.x = (55.004 * atan(readings.accel.x / sqrt(readings.accel.y * readings.accel.y + readings.accel.z + readings.accel.z))) + (0.04 * readings.gyro.x);
		readings.angle.x = (55.004 * atan(readings.accel.y / sqrt(readings.accel.x * readings.accel.x + readings.accel.z + readings.accel.z))) + (0.04 * readings.gyro.y);
		readings.angle.z = (55.04 * atan(sqrt(readings.accel.y * readings.accel.y + readings.accel.x + readings.accel.x) / readings.accel.z)) + (0.04 * readings.gyro.z);
		vTaskDelay(500/portTICK_PERIOD_MS);
	}
	
	xQueueSend(xQueueGyro, &readings, 0);

	vTaskDelay(pdMS_TO_TICKS(100));
}

