#ifndef __WEATHER_STATION_DEFS_H__
#define __WEATHER_STATION_DEFS_H__

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
    bool isRaining;
    bool isSunny;
} weather_data_t;

/**
 * @brief Initialize all sensors on the weather station
 * 
 * @return true -> Successful initialization
 * @return false -> Initialization failed
 */
bool ws_init_sensors();

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

/**
 * @brief Changes the data written to new clients on connection.
 * 
 * @param pData Pointer to a buffer of weather_data_t data
 */
void ws_website_set_data(const weather_data_t* pData);

/**
 * @brief Starts the weather station as an access point
 * 
 * @return true -> Success 
 * @return false -> Failure
 */
bool ws_start_soft_ap();

/**
 * @brief Starts the DNS & HTTP server, so clients can connect. Clients have to go to beccweather.com
 * in order to be redirected to the webpage.
 * 
 * @return true -> Success
 * @return false -> Failure
 */
bool ws_start_http_server();

/**
 * @brief Task to run periodically during loop. Contains processing functions for both DNS and HTTP
 * Note that this should run pretty fast, as it's what controls responding to clients. 
 * 
 */
void ws_server_task();

#endif