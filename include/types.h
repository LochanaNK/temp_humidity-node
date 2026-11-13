#pragma once
#include <Arduino.h>

//data structure declaration

struct __attribute__((packed)) TelemetryPayload
{
    uint32_t timestamp;
    float temperature;
    float humidity;
};