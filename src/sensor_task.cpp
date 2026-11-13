#include <Arduino.h>
#include "types.h"

void SensorTask(void *pvParameters)
{

    QueueHandle_t queue = (QueueHandle_t)pvParameters;

    TelemetryPayload data = {0, 20.0, 50.0};

    while (1)
    {
        data.timestamp = millis();
        data.temperature += random(-10, 40) / 10.0;
        data.humidity += random(-5, 6) / 10.0;

        if (xQueueSend(queue, &data, 0) != pdPASS)
        {
            Serial.println("Failed to send data to queue");
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}