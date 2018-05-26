#include "ad8232.h"

static void check_efuse()
{
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}

void ad8232_read_task(void * pvParameter)
{
	check_efuse();
	//Configure ADC
    if (unit_ad8232 == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_11);
        adc1_config_channel_atten(channel_ad8232, atten_ad8232);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel_ad8232, atten_ad8232);
    }
	
	xQueueECG = xQueueCreate(500, sizeof(uint32_t));

	adc_chars_ad8232 = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit_ad8232, atten_ad8232, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars_ad8232);
    print_char_val_type(val_type);


	while(1)
	{
		for (uint16_t count = 0; count < 500; ++count)
		{
			uint16_t adc_reading = 0;
			for (int i = 0; i < NO_OF_SAMPLES; i++) 
				adc_reading += adc1_get_raw((adc1_channel_t)channel_ad8232);
			adc_reading /= NO_OF_SAMPLES;
			uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars_ad8232);

			xQueueSend(xQueueECG, &voltage, 0);
			
			vTaskDelay(pdMS_TO_TICKS(10));
			printf("%d\n", voltage);
        
		}
		//vTaskDelay(pdMS_TO_TICKS(10000));
	}


}
