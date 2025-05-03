#ifndef __WEATHER_STATION_DEFS_H__
#define __WEATHER_STATION_DEFS_H__

#include <Arduino.h>
#include <stdbool.h>

// CONFIGURATION

#define BME_SCK        (18)
#define BME_MISO       (19)
#define BME_MOSI       (23)
#define BME_CS         (5)
#define LED_PIN        (12)
#define RADIO_ADDR     (3)
#define TX_INTERVAL_MS (1800000)

/**
 * @brief Data structure to hold all of our weather station data
 * 
 * Why use a data structure?
 * - More organized
 * - Contigous (side-by-side) in memory
 * 
 */
typedef struct {
    float temp_c;       ///> Temperature measured by temperature sensor, in degrees C
    float pres_kpa;     ///> Pressure measured in kilo-Pascals
    float humid;        ///> Humidity, measured as % relative
    float gas_kohms;    ///> TODO
    float wet_analog;   ///> Analog value of water level sensor. Use to determine if moisture is present on sensor
    float light_analog; ///> Analog value of photo resistor connected to system. Use to determine light intensity
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

int ws_get_analog_water();

int ws_get_analog_light();

void ws_tx_data(int address, const String& data);

void ws_set_status_led(bool ledState);

/**
 * @brief Shall soft reset the controller operating the weather station
 * 
 */
void ws_reset_controller();

#endif
