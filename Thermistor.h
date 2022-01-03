/*
 *  Copyright 2020-2021 Fredrik Andersson <fredrik.k.andersson@gmail.com>
 *
 *  This work is licensed under the CC BY-NC-SA 4.0 International License.
 * 
 */

#pragma once
#include <chrono>
#include <iostream>
#include <cmath>
#include <functional>
#include <map>
#include <mutex>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

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

    /**
     * Returns a Thermistor object for the specified adc channel.
     * If the Thermistor has never been used, it will be created
     * and a separate task that continously updates the senor will
     * be created
     *
     * Note once a Thermistor has been created it can not be stoped 
     * or deleted.
     *
     * @param[in] adc channel the terminstor is using
     * 
     * @return a pointer to a Thermistor instance. This pointer should
     * not be freed or deleted by the client. 
     */
    static Thermistor* Get_Thermistor(adc1_channel_t channel);
    
    /**
     * Returns the lastest reading from the sensor i degree Celsius
     * 
     * @return the temperature read by the sensor 
     */
    float get_reading();

    /**
     * Returns the full history of sensor readings. The history is
     * updated every HISTORY_INTERVALL (900) seconds.
     * 
     * @return a vector of sensor readings. Each reading consists
     * of a time stamp (GMT) and the degree Celsius.
     */
    std::vector<std::pair<int64_t, float>> get_history();

    /**
     * Returns the sensor range
     * 
     * @return tuple consisting of:
     *        minimum temperature
     *        maximum temperature
     *        minimum temperature step (this is arbitrarily set to 0.1)
     */
    std::tuple<float, float, float> get_sensor_range() const;

  private:
    adc1_channel_t m_adc_channel;
    adc_atten_t m_atten;
    esp_adc_cal_characteristics_t m_adc_cal_data;
    float m_reading = 0;
    std::mutex m_mutex;
    std::vector<std::pair<int64_t, float>> m_history;

    const float SENSOR_MIN = -55.0f;
    const float SENSOR_MAX = 125.0f;
    const float SENSOR_STEP = 0.1f;
    const uint32_t ADC_VREF = 1100;
    const uint32_t NO_OF_SAMPLES = 64;
    const float THERMISTORNOMINAL = 10000.0f;
    const float TEMPERATURENOMINAL = 25.0f;
    const int BCOEFFICIENT = 3950;
    const float SERIESRESISTOR = 10000.0f;
    const float VCC = 3300.0f;
    const int MAX_HISTORY_LENGHT = 192;
    const int HISTORY_INTERVALL = 900; // seconds

    static std::map<adc1_channel_t, Thermistor*> channel_list;

    Thermistor() = delete;
    Thermistor(adc1_channel_t adc, adc_atten_t attenuation);
    ~Thermistor() {};

    void update();
};
