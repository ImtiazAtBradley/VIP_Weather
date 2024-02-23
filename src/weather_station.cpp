/**
 * @file port.c
 * @brief This is the file where we will define all of our functions from
 * weather_station.h
 *
 */
#include <Arduino.h>

#include <DHTesp.h>

#include <weather_station.h>

//static variables???

static DHTesp dht = DHTesp();

//private functions???
//define functions of variables, easy to repeat later in code, easy to change if need be
float readTemperature() 
{
  return bme280.readTemperature(); //change bme280 to something else idk
}
float readHumididty()
{
  return bme280.readHumidity();
}
float readPressure()
{
  return bme280.readPressure() / 1000.0f;
}
//I will not define the isRaining or isSunny functions because I don't have to worry about them
bool isRaining(){} 
bool isSunny(){}

// CONNECT THE DHT11 SIGNAL TO PIN #8!!!!!!!
bool hal_setup_dht11() {
  dht.setup(8, DHTesp::DHT11);
  if (dht.getStatus() != DHTesp::ERROR_NONE) {
    return false;
  }

  return true;
}

/**
 * \brief Gets the temperature from hardware of the DHT11 sensor
 *
 * \return float Temperature,in degrees celsius.
 */
float hal_get_temperature_dht11() { return dht.getTemperature(); }

/**
 * \brief Gets the humidity from hardware of the DHT11 sensor
 *
 * \return float Humidity, in percent
 */
float hal_get_humidity_dht11() { return dht.getHumidity(); }

/* ========================================================================== */
/*                                 PUBLIC API                                 */
/* ========================================================================== */

/**
 * These are the functions that code from other files will call to use the
 * weather station you should try to put device-specific code int he above
 * section, "PRIVATE DEFINITIONS".
 *
 * REMEMBER: This is the "black box" other code uses to make the weather station
 * do stuff. Everything that includes "weather_station.h" can see these
 */

/**
 * @brief Initialize all sensors on the weather station
 *
 * @return true -> Successful initialization
 * @return false -> Initialization failed
 */
bool ws_init_sensors() { return hal_setup_dht11(); }

/**
 * @brief Read temperature from a sensor
 *
 * @return float Temperature, in degrees celsius
 */
float ws_get_temperature() {
  /**
   *  EMILY & SAMANTHA:
   *
   * Implement this function. Read the description to see what you need to
   * return. There should be a function above that suits your needs. Remember:
   * functions can return stuff so putting 'return' and then a function call
   * will return the value of that function.
   *
   * (see ws_init_sensors for an example)
   */

  return -1;
}

/**
 * @brief Read humidity from a sensor
 *
 * @return float Humidity, as a percent
 */
float ws_get_humidity() {
  /**
   *  EMILY & SAMANTHA:
   *
   *  Implement this as well
   */

  return -1;
}

/**
 * @brief Read pressure from a sensor
 *
 * @return float Pressure, in kPa
 */

float ws_get_pressure() {
  /**
   *  EMILY & SAMANTHA:
   *
   * You can't make this function work right now... go ahead and skip it until
   * we get a new sensor
   */
  return -1;
}

=====================================================================================================
        /* break between prewritten code and idk, stuff that we wrote in past weeks ig */
=====================================================================================================
/*  
  typedef struct //structure, define variables
{
  float temp_c; //temperature
  float pres_kpa; //pressure, in kpa(must convert to kpa)
  float humidity; //humidity
  bool raining; //raining or not raining, not stuff I have to worry about
  bool sunny; // sunny or cloudy, also not stuff i have to worry about
} weather_data_t;

#include <Arduino.h> //include libraries
#include <Adafruit_BME280.h>

//define functions of variables, easy to repeat later in code, easy to change if need be
float readTemperature() 
{
  return bme280.readTemperature();
}
float readHumididty()
{
  return bme280.readHumidity();
}
float readPressure()
{
  return bme280.readPressure() / 1000.0f;
}

//I will not define these functions because I don't have to worry about them
bool isRaining(){} 
bool isSunny(){}

weather_data_t weatherData ={0}; //set the weather data equal to zero so it doesn't give trash
Adafruit_BME280 bme280;


void setup() 
{
  weatherData.temp_c = readTemperature();
  Wire.begin(21, 22, 1000);
  Serial.begin(115200);
  bme280.begin();
}

void loop() 
{
  Serial.println(bme280.readTemperature());
  delay(500);
}
*/
