#include "Thermistor.h"


extern "C" void init_sensor(adc1_channel_t adc_channel) {
  // This will instantiate the thermistor and start update task
  Thermistor::Get_Thermistor(adc_channel);
}

extern "C" float get_sensor_reading(adc1_channel_t adc_channel) {
  return Thermistor::Get_Thermistor(adc_channel)->get_reading();
}
