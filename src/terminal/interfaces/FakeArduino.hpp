#pragma once

void delay(unsigned long ms) { return; }
void yield() {}
int64_t esp_timer_get_time() { return(0); }

using BaseType_t = long;
using TaskFunction_t = void (*)(void *);
using TaskHandle_t = void *;
using UBaseType_t = unsigned long;

void xTaskCreatePinnedToCore(TaskFunction_t pvTaskCode,
                                    const char * const pcName,
                                    const uint32_t usStackDepth,
                                    void * const pvParameters,
                                    UBaseType_t uxPriority,
                                    TaskHandle_t * const pxCreatedTask,
                                    const BaseType_t xCoreID) {}

class EspClass {
 public:
  void restart() { return; };
  ~EspClass() {}
};