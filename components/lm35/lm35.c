#include "lm35.h"



void lm35_read_task(void * pvParameter)
{
	xQueueTemperature = xQueueCreate(1, sizeof(uint32_t));
   
   	adc1_config_width(ADC_WIDTH_BIT_11);
    adc1_config_channel_atten(channel, atten);
    
	//Characterize ADC
	adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);


    //Continuously sample ADC1
    while (1) {
        uint16_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
			adc_reading += adc1_get_raw((adc1_channel_t)channel);
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV

        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
		uint32_t temperature = voltage / 10;
//        printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
//        vTaskDelay(pdMS_TO_TICKS(1000));
		xQueueSend(xQueueTemperature, &temperature, 0);
        vTaskDelay(pdMS_TO_TICKS(LM35_DELAY_TIME));
    }

}
