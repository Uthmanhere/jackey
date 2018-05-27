#include "mpu6050.h"


#define PIN_SDA 21
#define PIN_CLK 22
#define I2C_ADDRESS 0x68 // I2C address of MPU6050

#define MPU6050_ACCEL_XOUT_H 0x3B 
#define MPU6050_PWR_MGMT_1   0x6B

#define MPU6050_READING_SIZE	25
#define MPU6050_PRECISION_TIME	200
//#define MPU6050_DELAY_TIME		


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
	xQueueGyro = xQueueCreate(3 * MPU6050_READING_SIZE, sizeof(uint16_t));
	struct mpu6050_readings readings;
	while(1)
	{
//		for (int reading_count = 0; reading_count < MPU6050_READINGS_SIZE; ++reading_count) {
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

		
		readings.accel.x = (65536 - ((data[0] << 8) | data[1])) / 16384;
		readings.accel.y = (65536 - ((data[2] << 8) | data[3])) / 16384;
		readings.accel.z = (65536 - ((data[4] << 8) | data[5])) / 16384;


		readings.temperature = ((data[6] << 8) | data[7]) / 340 + 36.53;

		readings.gyro.x = 65536 - ((data[8] << 8) | data[9]);
		readings.gyro.y = 65536 - ((data[10] << 8) | data[11]);
		readings.gyro.z = 65536 - ((data[12] << 8) | data[13]);
	/*	
		readings.angle.x = (55.004 * atan(readings.accel.x / sqrt(readings.accel.y * readings.accel.y + readings.accel.z + readings.accel.z))) + (0.04 * readings.gyro.x);
		readings.angle.x = (55.004 * atan(readings.accel.y / sqrt(readings.accel.x * readings.accel.x + readings.accel.z + readings.accel.z))) + (0.04 * readings.gyro.y);
		readings.angle.z = (55.04 * atan(sqrt(readings.accel.y * readings.accel.y + readings.accel.x + readings.accel.x) / readings.accel.z)) + (0.04 * readings.gyro.z);
	*/
		//printf("Accelerations: %d %d %d\t Gyro: %d %d %d Angle: %d %d %d\n", readings.accel.x, readings.accel.y, readings.accel.z, readings.gyro.x, readings.gyro.y, readings.gyro.z, readings.angle.x, readings.angle.y, readings.angle.z);
		xQueueSend(xQueueGyro, &readings.gyro.x, 0);
		xQueueSend(xQueueGyro, &readings.gyro.y, 0);
		xQueueSend(xQueueGyro, &readings.gyro.z, 0);
		vTaskDelay(MPU6050_PRECISION_TIME / portTICK_PERIOD_MS);

	}
	
	
}

