#pragma once
#include <driver/adc.h>

void init_sensor(adc1_channel_t adc_channel);
float get_sensor_reading(adc1_channel_t adc_channel);
