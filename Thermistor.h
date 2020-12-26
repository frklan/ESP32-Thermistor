#pragma once
#include <iostream>
#include <cmath>
#include <functional>
#include <map>
#include <mutex>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <sdkconfig.h>

#include <hal/adc_types.h>
#include <driver/adc.h>

#include "Sensor_Task.h"


class Thermistor: protected Sensor_Task {
  public:
    static Thermistor* Get_Thermistor(adc1_channel_t channel);
    
    float get_reading();

  private:
    adc1_channel_t m_adc;
    adc_atten_t m_atten;
    float m_reading = 0;
    std::mutex m_mutex;

    static std::map<adc1_channel_t, Thermistor*> channel_list;

    Thermistor() = delete;
    Thermistor(adc1_channel_t adc, adc_atten_t attenuation);

    void update();
};
