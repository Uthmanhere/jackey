#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#ifndef LM35_H
#define LM35_H

#define DEFAULT_VREF	1100
#define NO_OF_SAMPLES   64          //Multisampling
#define LM35_DELAY_TIME	5000

xQueueHandle xQueueTemperature;

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_6;
static const adc_unit_t unit = ADC_UNIT_1;


void lm35_read_task(void *);

#endif
