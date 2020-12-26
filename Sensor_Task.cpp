#include "Sensor_Task.h"

Sensor_Task::Sensor_Task() : Sensor_Task(5000) {}

Sensor_Task::Sensor_Task(uint32_t task_period_ms) : m_task_period_ms(task_period_ms) {
  start_task();
}

Sensor_Task::~Sensor_Task() {
  if(m_update_task != nullptr) {
    vTaskDelete(m_update_task);
    m_update_task = nullptr;
  }
}

void Sensor_Task::start_task() {
  xTaskCreatePinnedToCore([](void* arg){
    auto self  = static_cast<Sensor_Task*>(arg);
    
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t delay = self->m_task_period_ms / portTICK_PERIOD_MS;

    while(true) {
      self->update();
      vTaskDelayUntil(&xLastWakeTime, delay);
    }
    vTaskDelete(NULL);
  }, "sensor task", 4096, this, 1, &m_update_task, 1);
}

