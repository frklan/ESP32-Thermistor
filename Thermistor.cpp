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

Thermistor::Thermistor(adc1_channel_t adc, adc_atten_t attenuation) :
m_adc(adc),
m_atten(attenuation) {
  adc_gpio_init(ADC_UNIT_1, ADC_CHANNEL_6);
  adc1_config_channel_atten(m_adc, m_atten);
  adc1_config_width(ADC_WIDTH_BIT_12);

  // update once
  update();
}

void Thermistor::update() {
  const int THERMISTORNOMINAL = 10000;
  const int TEMPERATURENOMINAL = 22;
  const int BCOEFFICIENT = 3950;
  const int SERIESRESISTOR = 10000;

  auto raw = adc1_get_raw(ADC1_CHANNEL_6);

  float r =  (float) SERIESRESISTOR / (4095.0f / (float) raw - 1.0f);
  float steinhart;
  steinhart = r / THERMISTORNOMINAL;
  steinhart = log(steinhart);
  steinhart /= BCOEFFICIENT;
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15);
  steinhart = 1.0 / steinhart;

  std::lock_guard guard(m_mutex);
  m_reading = steinhart - 273.15;
}
