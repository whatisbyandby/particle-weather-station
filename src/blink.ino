#include "MQTT.h"
#include "SparkFun_Photon_Weather_Shield_Library.h"
#include "WeatherData.h"

//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barometric sensor
Weather sensor;
SerialLogHandler logHandler;
// SystemSleepConfiguration config;

void callback(char *topic, byte *payload, unsigned int length);
MQTT client("home-automation.perkylab.com", 1883, callback);
WeatherData weatherData;

void callback(char *topic, byte *payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = 0;

  Log.info("Message Received");
}

// The following line is optional, but recommended in most firmware. It
// allows your code to run before the cloud is connected. In this case,
// it will begin blinking almost immediately instead of waiting until
// breathing cyan,
SYSTEM_THREAD(ENABLED);

//---------------------------------------------------------------
void setup()
{

  // RGB.control(true); // Turn off the LED

  //Initialize the I2C sensors and ping them
  sensor.begin();

  /*You can only receive accurate barometric readings or accurate altitude
    readings at a given time, not both at the same time. The following two lines
    tell the sensor what mode to use. You could easily write a function that
    takes a reading in one made and then switches to the other mode to grab that
    reading, resulting in data that contains both accurate altitude and barometric
    readings. For this example, we will only be using the barometer mode. Be sure
    to only uncomment one line at a time. */
  sensor.setModeBarometer(); //Set to Barometer Mode
  // sensor.setModeAltimeter(); //Set to altimeter Mode

  //These are additional MPL3115A2 functions that MUST be called for the sensor to work.
  sensor.setOversampleRate(7); // Set Oversample rate
  //Call with a rate from 0 to 7. See page 33 for table of ratios.
  //Sets the over sample rate. Datasheet calls for 128 but you can set it
  //from 1 to 128 samples. The higher the oversample rate the greater
  //the time between data samples.

  sensor.enableEventFlags(); //Necessary register calls to enble temp, baro and alt

  client.connect("weather-station"); // connect to the server
}
//---------------------------------------------------------------
void loop()
{
  //Get readings from all sensors
  updateWeather();
  // if (client.isConnected())
  //   client.loop();
  delay(1000);
}
//---------------------------------------------------------------
void updateWeather()
{
  // Measure Relative Humidity from the HTU21D or Si7021
  float humidity = sensor.getRH();

  // Measure Temperature from the HTU21D or Si7021
  float tempf = sensor.getTempF();
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()

  //Measure the Barometer temperature in F from the MPL3115A2
  float baroTemp = sensor.readBaroTempF();

  //Measure Pressure from the MPL3115A2
  float pascals = sensor.readPressure();
  bool bufferFull = weatherData.addSample(tempf, humidity, pascals);
  if (bufferFull)
  {
    // Create a buffer for the JSON string
    Log.info("Buffer full");
    char buffer[1024];
    // Create a JSON object
    memset(buffer, 0, sizeof(buffer));
    JSONBufferWriter writer(buffer, sizeof(buffer) - 1);

    writer.beginObject();
    writer.name("temperature").value(weatherData.getTemperature());
    writer.name("humidity").value(weatherData.getHumidity());
    writer.name("pressure").value(weatherData.getPressure());
    writer.endObject();
    Serial.println(buffer);
    if (!client.isConnected())
    {
      client.connect("weather-station");
    }

    bool published = client.publish("environment/weather-station", buffer);
    if (published)
    {
      Log.info("Published");
    }
    else
    {
      Log.info("Not published");
    }
  }

  //If in altitude mode, you can get a reading in feet with this line:
  // altf = sensor.readAltitudeFt();
  // Log.info("Reading Sensors");
}
//---------------------------------------------------------------
