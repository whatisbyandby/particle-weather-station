#include "WeatherData.h"
#include "MQTT.h"

WeatherData::WeatherData(){};

void WeatherData::setClient(MQTT client)
{
    this->client = client;
}

bool WeatherData::addSample(float temperature, float humidity, float pressure)
{
    bool bufferFull = this->temperatureBuffer.push(temperature);
    this->humidityBuffer.push(humidity);
    this->pressureBuffer.push(pressure);
    return bufferFull;
}

void WeatherData::printSamples()
{
    Log.info("Temperature: %f", getTemperature());
    Log.info("Humidity: %f", getHumidity());
    Log.info("Pressure: %f", getTemperature());
}

float WeatherData::getTemperature()
{
    return this->temperatureBuffer.getAverage();
}

float WeatherData::getHumidity()
{
    return this->humidityBuffer.getAverage();
}

float WeatherData::getPressure()
{
    return this->pressureBuffer.getAverage();
}
