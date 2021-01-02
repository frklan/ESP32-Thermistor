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
}
