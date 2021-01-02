#pragma once
#include <iostream>
#include <cmath>
#include <functional>
#include <map>
#include <mutex>
#include <thread>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <sdkconfig.h>

#include <hal/adc_types.h>
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "Sensor_Task.h"


class Thermistor: protected Sensor_Task {
  public:
    static Thermistor* Get_Thermistor(adc1_channel_t channel);
    
    float get_reading();

  private:
    adc1_channel_t m_adc_channel;
    adc_atten_t m_atten;
    esp_adc_cal_characteristics_t m_adc_cal_data;
    float m_reading = 0;
    std::mutex m_mutex;

    const uint32_t ADC_VREF = 1100;
    const uint32_t NO_OF_SAMPLES = 64;
    const float THERMISTORNOMINAL = 10000.0f;
    const float TEMPERATURENOMINAL = 25.0f;
    const int BCOEFFICIENT = 3950;
    const float SERIESRESISTOR = 10000.0f;
    const float VCC = 3300.0f;

    static std::map<adc1_channel_t, Thermistor*> channel_list;

    Thermistor() = delete;
    Thermistor(adc1_channel_t adc, adc_atten_t attenuation);

    void update();
};
