#include "MQTT.h"
#include "CircularBuffer.h"

class WeatherData
{
private:
    const int maxSamples = 10;
    MQTT client;
    CircularBuffer temperatureBuffer = CircularBuffer(maxSamples);
    CircularBuffer humidityBuffer = CircularBuffer(maxSamples);
    CircularBuffer pressureBuffer = CircularBuffer(maxSamples);

public:
    WeatherData();
    void setClient(MQTT client);
    bool addSample(float temperature, float humidity, float pressure);
    void averageSamples();
    void printSamples();
    void toJson(char *buffer);
    float getTemperature();
    float getHumidity();
    float getPressure();
};