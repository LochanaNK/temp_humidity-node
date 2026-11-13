#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include "tasks.h"
#include "types.h"

QueueHandle_t telemetryQueue;

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("Botting temp and humidity node...");

  telemetryQueue = xQueueCreate(10, sizeof(TelemetryPayload));

  if (telemetryQueue == NULL)
  {
    while (true)
    {
      Serial.println("Failed to create telemetry queue");
    }
  }

  xTaskCreate(SensorTask, "Sensor Task", 2048, (void *)telemetryQueue, 1, NULL);
  xTaskCreate(NetworkTask, "Network Task", 4096, (void *)telemetryQueue, 1, NULL);
}

void loop()
{
  vTaskDelete(NULL);
}
