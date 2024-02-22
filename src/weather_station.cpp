/**
 * @file port.c
 * @brief This is the file where we will define all of our functions from
 * weather_station.h
 *
 */
#include <Arduino.h>

#include <DHTesp.h>

#include <weather_station.h>

/* ========================================================================== */
/*                              STATIC VARIABLES                              */
/* ========================================================================== */

static DHTesp dht = DHTesp();

/* ========================================================================== */
/*                              PRIVATE FUNCTIONS                             */
/* ========================================================================== */

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