#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"



#ifndef AD8232_H
#define AD8232_H

#define DEFAULT_VREF	1100
#define NO_OF_SAMPLES   64          //Multisampling

static esp_adc_cal_characteristics_t *adc_chars_ad8232;
static const adc_channel_t channel_ad8232 = ADC_CHANNEL_7;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten_ad8232 = ADC_ATTEN_DB_6;
static const adc_unit_t unit_ad8232 = ADC_UNIT_1;

xQueueHandle xQueueECG;

void ad8232_read_task(void *);

#endif
