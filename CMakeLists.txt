idf_component_register(SRCS "Thermistor.cpp" "Sensor_Task.cpp"
                    INCLUDE_DIRS "." 
                    REQUIRES driver esp_adc_cal freertos
                    )

