/*
 *  Copyright 2020-2021 Fredrik Andersson <fredrik.k.andersson@gmail.com>
 *
 *  This work is licensed under the CC BY-NC-SA 4.0 International License.
 * 
 */

#ifndef TERMISTORINTEROPH
#define TERMISTORINTEROPH

#include <driver/adc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Starts and inits a Thermistor sensor
 * 
 * Note this function will create a separate task that keeps
 * updating the sensor internals.
 * 
 * @param[in] adc_channel HW adc channel the thermistor is connected to 
 */
void init_sensor(adc1_channel_t adc_channel);

/**
 * Return the las read temperature value on a specific thermistor channel
 * 
 * @param[in] adc_channel HW adc channel the thermistor is connected to 
 */
float get_sensor_reading(adc1_channel_t adc_channel);

/** Sensor History */
struct sensor_history_t {
  /** Time stamp of the reading, formatted as a Unix Epoch time stamp */
  int64_t time;
  
  /** Current reading in degree Celcius */
  float reading;
};

/**
 * Returns the sensor reading history
 * 
 * @param[in] adc_channel HW adc channel the thermistor is connected to
 * @param[out] history pointer to an allocated \ref sensor_history_t 
 * structure which will be populated with the history data
 * @param[in] max_count size of the history buffer
 */
unsigned long long get_sensor_history(adc1_channel_t adc_channel, struct sensor_history_t* history, size_t max_count);

/**
 * Returns the sensor range
 * 
 * @param[in] adc_channel HW adc channel the thermistor is connected to
 * @param[out] range_min minimum temperature
 * @param[out] range_max maximum temperature
 * @param[out] range_step minimum temperature step
 */
void get_sensor_range(adc1_channel_t adc_channel, float *range_min, float* range_max, float* range_step);

#ifdef __cplusplus
}
#endif

#endif
