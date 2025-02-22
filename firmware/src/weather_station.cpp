/**
 * @file port.c
 * @author Owen J., Jacob S.
 * @brief This is the file where we will define all of our functions from
 * weather_station.h
 *
 */

#include <cstdio>
#include <secrets.h>
#include <weather_station.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <weather_station.h>

#define LED_PIN (19)

/* ========================================================================== */
/*                              STATIC VARIABLES                              */
/* ========================================================================== */

// BME280
Adafruit_BME280 bme; // I2C

/* ========================================================================== */
/*                              PRIVATE FUNCTIONS                             */
/* ========================================================================== */

int
hal_get_light_level() {
    //lightPin = 32 max: 4095, min: 0
    return analogRead(32);
}

int
hal_get_water_level() {
    //waterPin = 33 max: ?,    min: 0
    return analogRead(33);
}

void
hal_setup_digital() {
    pinMode(LED_PIN, OUTPUT);
}

bool
hal_setup_bme() {
    return bme.begin(BME280_ADDRESS_ALTERNATE);
}

/**
 * \brief Gets the temperature from hardware of the DHT11 sensor
 *
 * \return float Temperature,in degrees celsius.
 */
float
hal_get_temperature_bme() {
    return bme.readTemperature();
}

/**
 * \brief Gets the humidity from hardware of the DHT11 sensor
 *
 * \return float Humidity, in percent
 */
float
hal_get_humidity_bme() {
    return bme.readHumidity();
}

/**
 * @brief Read the current pressure, from the bme280
 * 
 * @return float Pressure, in pascals
 */
float
hal_get_pressure_bme() {
    return bme.readPressure();
}

void
reset_radio(int rstPin) {

    digitalWrite(rstPin, LOW);
    // 5 ms should be more than enough
    delay(100);
    digitalWrite(rstPin, HIGH);
}

void
send_cmd(const String& str) {
    Serial1.print(str + "\r\n");
}

// True on success, false on fail - quick & dirty
bool
set_address(int address) {
    int retry = 0;
    int rxAttempts = 0;
    String rx;
    while (retry <= 3) {
        send_cmd("AT+ADDRESS=" + String(address));
        // Wait for response
        while (rxAttempts <= 10) {
            rx += Serial1.readString();
            rx.trim();
            if (rx == "+OK") {
                // Successfully configured
                return true;
            }
            delay(100);
            rxAttempts++;
        }

        retry++;
    }

    return false;
}

void
send_data(const int address, const String& data) {
    String command = "AT+SEND=" + String(address) + "," + String(data.length()) + "," + data;
    send_cmd(command);
}

void
hal_set_led(bool ledState) {
    digitalWrite(LED_PIN, ledState);
}

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
bool
ws_init() {

    hal_setup_digital();

    hal_set_led(1);

    Wire.begin(21, 22, 5000);

    Serial1.begin(115200);

    // Setup radio
    reset_radio(13);
    if (!set_address(0)) {
        return false;
    }

    return hal_setup_bme();
}

/**
 * @brief Read temperature from a sensor
 *
 * @return float Temperature, in degrees celsius
 */
float
ws_get_temperature() {

    return hal_get_temperature_bme();
}

/**
 * @brief Read humidity from a sensor
 *
 * @return float Humidity, as a percent
 */
float
ws_get_humidity() {
    return hal_get_humidity_bme();
}

/**
 * @brief Read pressure from a sensor
 *
 * @return float Pressure, in kPa
 */
float
ws_get_pressure() {
    return hal_get_pressure_bme() / 1000.0f; // Convert o kPa
}

bool
ws_is_raining() {
    if (hal_get_water_level() > 0) {
        return true;
    } else {
        return false;
    }
}

bool
ws_is_sunny() {
    return hal_get_light_level() > 850;
}

int
ws_raw_raining() {
    return hal_get_water_level();
}

int
ws_raw_light() {
    return hal_get_light_level();
}

void
ws_tx_data(int address, const String& data) {
    send_data(address, data);
}

void
ws_set_status_led(bool ledState) {
    hal_set_led(ledState);
}

void
ws_reset_controller() {
    // Perform a soft restart of the ESP32
    ESP.restart();
}
