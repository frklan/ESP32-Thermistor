/*
 *  Copyright 2020-2021 Fredrik Andersson <fredrik.k.andersson@gmail.com>
 *
 *  This work is licensed under the CC BY-NC-SA 4.0 International License.
 * 
 */

#pragma once
#include <iostream>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

class Sensor_Task {
  public:
    Sensor_Task();
    Sensor_Task(uint32_t task_period_ms);

    virtual ~Sensor_Task();

    virtual void update() = 0;

  private:
    uint32_t m_task_period_ms = 0;
    TaskHandle_t m_update_task = nullptr;

    void start_task();
};
