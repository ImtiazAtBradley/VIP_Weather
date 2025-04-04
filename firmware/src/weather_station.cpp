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
#include <Adafruit_BME680.h>
#include <weather_station.h>
#include "PwFusion_MAX31865.h"

#define SPI_SCK  (18)
#define SPI_MISO (19)
#define SPI_MOSI (23)
#define RTD_CS   (22)
#define BME_CS   (5)
#define LED_PIN  (12)
#define RADIO_RST (13)
#define LIGHT_PIN (32)
#define WATER_PIN (35)

/* ========================================================================== */
/*                              STATIC VARIABLES                              */
/* ========================================================================== */

// BME280
Adafruit_BME680 bme680(BME_CS, &SPI);

// RTD BOARD (MAX31865)
MAX31865 rtd;

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
    return analogRead(WATER_PIN);
}

void
hal_setup_digital() {
    pinMode(BME_CS, OUTPUT);
    pinMode(RTD_CS, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
}

bool
hal_setup_bme() {
    bool rc = !bme680.begin();

    rc |= !bme680.setTemperatureOversampling(BME680_OS_8X);
    rc |= !bme680.setHumidityOversampling(BME680_OS_2X);
    rc |= !bme680.setPressureOversampling(BME680_OS_4X);
    rc |= !bme680.setIIRFilterSize(BME680_FILTER_SIZE_3);
    rc |= !bme680.setGasHeater(320, 150);

    return !rc;
}

bool
hal_setup_rtd_board() {
    rtd.begin(RTD_CS, RTD_4_WIRE, RTD_TYPE_PT100);
    return true;
}

/**
 * @brief Get temperature from MAX31865
 * 
 * @return float Temperature, in degrees C
 */
float
hal_get_rtd_temp() {
    // Read from register
    rtd.sample();
    // NOTE: Using linear approximation and not the Callendar–Van Dusen equation
    return rtd.getTemperature();
}

/**
 * \brief Gets the humidity from hardware of the DHT11 sensor
 *
 * \return float Humidity, in percent
 */
float
hal_get_humidity_bme() {
    return bme680.readHumidity();
}

/**
 * @brief Read the current pressure, from the bme280
 * 
 * @return float Pressure, in pascals
 */
float
hal_get_pressure_bme() {
    return bme680.readPressure();
}

//returns gas sensor analog value in ohms
uint32_t
hal_get_gas_sensor() {
    return bme680.readGas();
}

void
reset_radio(int rstPin) {

    digitalWrite(rstPin, LOW);
    // 100 ms as per datasheet + a little
    delay(100);
    digitalWrite(rstPin, HIGH);
}

void
send_cmd(const String& str) {
    Serial2.print(str + "\r\n");
}

// True on success, false on fail - quick & dirty
bool
set_address(int address) {
    int retry = 0;
    int rxAttempts = 0;
    String rx;

    send_cmd("AT+ADDRESS=" + String(address));
    // Wait before reading
    delay(1000);

    rx = Serial2.readString();
    rx.trim();

    while (rx != "+OK")
    {
        send_cmd("AT+ADDRESS=" + String(address));
        delay(1000);
        rx = Serial2.readString();
    }

    return true;
}

void
send_data(const int address, const String& data) {
    String command = "AT+SEND=" + String(address) + "," + String(data.length()) + "," + data;
    send_cmd(command);
}

void
hal_set_stat_led(bool ledState) {
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
    bool ret = true;

    hal_setup_digital();

    hal_set_stat_led(1);

    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    Serial2.begin(115200);

    // Setup radio
    reset_radio(RADIO_RST);
    ret |= set_address(0);

    ret |= hal_setup_rtd_board();
    ret |= hal_setup_bme();

    return ret;
}

/**
 * @brief Read temperature from a sensor
 *
 * @return float Temperature, in degrees celsius
 */
float
ws_get_temperature() {

    return hal_get_rtd_temp();
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

float
ws_get_gas_sensor() {
    return hal_get_gas_sensor() / 1000.0f; //Convert to kOhms
}

int
ws_get_analog_water() {
    return hal_get_water_level();
}

int
ws_get_analog_light() {
    return hal_get_light_level();
}

void
ws_tx_data(int address, const String& data) {
    send_data(address, data);
}

void
ws_set_status_led(bool ledState) {
    hal_set_stat_led(ledState);
}

void
ws_reset_controller() {
    // Perform a soft restart of the ESP32
    ESP.restart();
}
