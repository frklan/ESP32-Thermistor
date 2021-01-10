/*
 *  Copyright 2020-2021 Fredrik Andersson <fredrik.k.andersson@gmail.com>
 *
 *  This work is licensed under the CC BY-NC-SA 4.0 International License.
 * 
 */

#include "Thermistor.h"

std::map<adc1_channel_t, Thermistor*> Thermistor::channel_list;

Thermistor* Thermistor::Get_Thermistor(adc1_channel_t channel)  {
  if(channel_list.find(channel) == channel_list.end()) {
    channel_list[channel] = new Thermistor(channel, ADC_ATTEN_DB_11);
  }
  return channel_list[channel];
}

float Thermistor::get_reading() {
  std::lock_guard guard(m_mutex);
  return m_reading;
}
std::vector<std::pair<int64_t, float>> Thermistor::get_history() {
  std::lock_guard guard(m_mutex);
  return m_history;
}

std::tuple<float, float, float> Thermistor::get_sensor_range() const {
  return std::make_tuple(SENSOR_MIN, SENSOR_MAX, SENSOR_STEP);
}

Thermistor::Thermistor(adc1_channel_t adc_channel, adc_atten_t attenuation) :
m_adc_channel(adc_channel),
m_atten(attenuation) {
  adc_gpio_init(ADC_UNIT_1, static_cast<adc_channel_t>(m_adc_channel));
  adc1_config_channel_atten(m_adc_channel, m_atten);
  adc1_config_width(ADC_WIDTH_BIT_12);

  esp_adc_cal_characterize(ADC_UNIT_1, 
    m_atten, 
    ADC_WIDTH_BIT_12, 
    ADC_VREF, 
    &m_adc_cal_data);
  
  m_history.reserve(MAX_HISTORY_LENGHT);

  // update once
  update();
}

void Thermistor::update() {
  uint32_t adc_reading = 0;
  for(int i = 0; i < NO_OF_SAMPLES; i++) {
    adc_reading += adc1_get_raw(m_adc_channel);
  }
  adc_reading /= NO_OF_SAMPLES;
  
  // Convert adc_reading to voltage in mV over thermistor (Rt)
  // with compensation to the non-linear adc.
  float Vt = static_cast<float>(esp_adc_cal_raw_to_voltage(adc_reading, &m_adc_cal_data));
  
  // Convert Volt to °C
  float Rt = SERIESRESISTOR / ((VCC / Vt) - 1);
  float steinhart = Rt / THERMISTORNOMINAL;
  steinhart = log(steinhart);
  steinhart /= BCOEFFICIENT;
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15);
  steinhart = 1.0 / steinhart;

  std::lock_guard guard(m_mutex);
  m_reading = steinhart - 273.15;

  // save history
  auto epoch_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  if(epoch_time < 1600000000) { // just a good looking number that we can assume never will
                                // happen unless we've actually set the time from a server,
    return;                     // and bail if time is not set
  }

  
  if(m_history.size() > 0) { // history empty -> always save value
    auto last_reading_timestamp = std::get<0>(m_history.back());
    // do not save reading to history if < HISTORY_INTERVALL seconds since last record
    if(last_reading_timestamp + HISTORY_INTERVALL > epoch_time) {
      return;
    }
  }

  m_history.push_back(std::make_pair(epoch_time, m_reading));

  // Prune oldest item if full
  if(m_history.size() > MAX_HISTORY_LENGHT) {
    m_history.erase(m_history.begin());
  }
}
