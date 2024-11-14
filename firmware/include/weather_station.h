#ifndef __WEATHER_STATION_DEFS_H__
#define __WEATHER_STATION_DEFS_H__

#include <Arduino.h>
#include <stdbool.h>

/**
 * @brief Data structure to hold all of our weather station data
 * 
 * Why use a data structure?
 * - More organized
 * - Contigous (side-by-side) in memory
 * 
 */
typedef struct {
    float temp_c;
    float pres_kpa;
    float humid;
    float gas_kohms;
    bool isRaining;
    bool isSunny;
} weather_data_t;

/**
 * @brief Initialize all sensors on the weather station
 * 
 * @return true -> Successful initialization
 * @return false -> Initialization failed
 */
bool ws_init();

/**
 * @brief Read temperature from a sensor
 * 
 * @return float Temperature, in degrees celsius
 */
float ws_get_temperature();

/**
 * @brief Read humidity from a sensor
 * 
 * @return float Humidity, as a percent
 */
float ws_get_humidity();

/**
 * @brief Read pressure from a sensor
 *
 * @return float Pressure, in kPa
 */
float ws_get_pressure();

float ws_get_gas_sensor();
 
/**
 * @brief Check if it is currently raining
 * 
 * @return true -> Currently raining
 * @return false -> Not currently raining
 */
bool ws_is_raining();

/**
 * @brief Check if it is currently sunny out
 * 
 * @return true -> Currently sunny
 * @return false -> Not currently sunny
 */
bool ws_is_sunny();

void ws_tx_data(int address, const String& data);

void ws_set_status_led(bool ledState);

int ws_raw_raining();

int ws_raw_light();

#endif
