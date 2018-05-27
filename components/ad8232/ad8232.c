#include "ad8232.h"


void ad8232_read_task(void * pvParameter)
{
	//Configure ADC
    if (unit_ad8232 == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_11);
        adc1_config_channel_atten(channel_ad8232, atten_ad8232);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel_ad8232, atten_ad8232);
    }
	
	xQueueECG = xQueueCreate(AD8232_READINGS_SIZE, sizeof(uint32_t));

	adc_chars_ad8232 = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit_ad8232, atten_ad8232, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars_ad8232);


	while(1)
	{
		uint16_t adc_reading = 0;
		for (int i = 0; i < NO_OF_SAMPLES; i++) 
			adc_reading += adc1_get_raw((adc1_channel_t)channel_ad8232);
		adc_reading /= NO_OF_SAMPLES;
		uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars_ad8232);

		xQueueSend(xQueueECG, &voltage, 0);
		
		vTaskDelay(pdMS_TO_TICKS(AD8232_PRECISION_TIME));
		//printf("%d\n", voltage);
        
	}


}
