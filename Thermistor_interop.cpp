/*
 *  Copyright 2020-2021 Fredrik Andersson <fredrik.k.andersson@gmail.com>
 *
 *  This work is licensed under the CC BY-NC-SA 4.0 International License.
 * 
 */

#include <tuple>

#include "Thermistor.h"
#include "Thermistor_interop.h"

#ifdef __cplusplus
extern "C" {
#endif

void init_sensor(adc1_channel_t adc_channel) {
  // This will instantiate the thermistor and start update task
  Thermistor::Get_Thermistor(adc_channel);
}

extern "C" float get_sensor_reading(adc1_channel_t adc_channel) {
  return Thermistor::Get_Thermistor(adc_channel)->get_reading();
}

unsigned long long get_sensor_history(adc1_channel_t adc_channel, sensor_history_t* history, size_t max_count) {
  auto sensor_history = Thermistor::Get_Thermistor(adc_channel)->get_history();

  size_t count_to_convert = std::min(max_count, sensor_history.size());
  size_t begin = sensor_history.size() - count_to_convert;

  for(size_t i = begin, p = 0; i < sensor_history.size(); i++, p++) {
      history[p].time = std::get<0>(sensor_history[i]);
      history[p].reading = std::get<1>(sensor_history[i]);
  }

  return std::min(max_count, sensor_history.size());
}

void get_sensor_range(adc1_channel_t adc_channel, float *range_min, float* range_max, float* range_step) {
  auto sensor_range = Thermistor::Get_Thermistor(adc_channel)-> get_sensor_range();
  *range_min = std::get<0>(sensor_range);
  *range_max = std::get<1>(sensor_range);
  *range_step = std::get<2>(sensor_range);
}


#ifdef __cplusplus
}
#endif
