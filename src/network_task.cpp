#include "types.h"
#include <WiFi.h>
#include "secrets.h"

const char *ssid = SECRET_WIFI_SSID;
const char *password = SECRET_WIFI_PASSWORD;

const char *host = SECRET_SERVER_HOST;
const uint16_t port = SECRET_SERVER_PORT;

void NetworkTask(void *pvParameters)
{

    QueueHandle_t queue = (QueueHandle_t)pvParameters;
    TelemetryPayload receivedData;

    const size_t bufferSize = sizeof(TelemetryPayload);
    uint8_t buffer[bufferSize];

    WiFiClient client;

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFI....");

    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        Serial.print(".");
    }

    Serial.println("\n WiFi connected :)");

    while (true)
    {
        if (xQueueReceive(queue, &receivedData, portMAX_DELAY) == pdPASS)
        {
            memcpy(buffer, (uint8_t *)&receivedData, bufferSize);

            if (WiFi.status() != WL_CONNECTED)
            {
                Serial.println("WiFi disconnected. Reconnecting...");
                WiFi.disconnect();
                WiFi.begin(ssid, password);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                continue;
            }
            if (!client.connected())
            {
                Serial.println("Connecting to server...");
                if (!client.connect(host, port))
                {
                    Serial.println("[TX Task] TCP connection failed.");
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                    continue;
                }
            }

            size_t bytesSent = client.write(buffer, bufferSize);

            Serial.print("Sent: ");
            for (size_t i = 0; i < bufferSize; i++)
            {
                Serial.printf("%02X", buffer[i]);
            }
            Serial.println();

            if (bytesSent != bufferSize)
            {
                Serial.println("[TX Task] Failed to send all bytes.");
                client.stop();
            }
            else
            {
                Serial.println("[TX Task] Data sent successfully.");
            }
        }
    }
}