/**
 * @file main.cpp
 * @brief VIP Project weather station source
 * 
 */
#include <Arduino.h>
#include <weather_station.h>

// #define DEBUG

/* ========================================================================== */
/*                                MAIN PROGRAM                                */
/* ========================================================================== */

#ifdef DEBUG
static long lastDebug = LONG_MAX;
#endif

static long lastTransmit = LONG_MAX;

void
setup() {
    Serial.begin(115200);

    Serial.println("BOOTING...");

    delay(500);

    // ws_init also turns on status LED
    if (!ws_init()) {
        // Blink a couple times to notify user
        for (int i = 0; i < 3; ++i) {
            ws_set_status_led(false);
            delay(500);
            ws_set_status_led(true);
            delay(500);
        }

        Serial.println("BOOT RTRY");
        // Wait before soft restart so user can see above message
        delay(1000);
        // Perform a soft restart
        ws_reset_controller();
    }

    Serial.println("BOOT OK");

    // Turn off status LED after boot
    // delay to make it clear this was the boot exiting
    ws_set_status_led(false);
    delay(1000);
}

void
loop() {
    static weather_data_t wd = {0};
    float temp_c = 0.0f, humidity = 0.0f, pres_kpa = 0.0f, gas_kohms = 0.0f, an_wet = 0.0f, an_light = 0.0f;

    temp_c = ws_get_temperature();
    an_wet = ws_get_analog_water();
    humidity = ws_get_humidity();
    pres_kpa = ws_get_pressure();
    gas_kohms = ws_get_gas_sensor();
    an_light = ws_get_analog_light();

    wd.temp_c = temp_c;
    wd.wet_analog = an_wet;
    wd.humid = humidity;
    wd.pres_kpa = pres_kpa;
    wd.gas_kohms = gas_kohms;
    wd.light_analog = an_light;

#ifdef DEBUG
    if (millis() - lastDebug > 1000) {
        // Blink to indicate it's not dead
        ws_set_status_led(true);
        delay(20);
        ws_set_status_led(false);
        // Print out some debug stuff
        Serial.println("[DEBUG] CURRENT VALUES: ");
        Serial.print("Water Level..........");
        Serial.print(wd.wet_analog);
        Serial.print("\n");
        Serial.print("Light Level..........");
        Serial.print(wd.light_analog);
        Serial.print("\n");
        Serial.print("Temperature..........");
        Serial.print(wd.temp_c);
        Serial.print("\n");
        Serial.print("Pressure Level.......");
        Serial.print(wd.pres_kpa);
        Serial.print("\n");
        Serial.print("Humidity Level.......");
        Serial.print(wd.humid);
        Serial.print("\n");
        Serial.print("Gas Sensor...........");
        Serial.print(wd.gas_kohms);
        Serial.print("\n");

        Serial.print("........................................\n");

        lastDebug = millis();
    }
#endif

    if (millis() - lastTransmit > TX_INTERVAL_MS) {

        String temp_c, humid, pres_kpa, wet, light, gas_kohms;
        temp_c = String(wd.temp_c);
        humid = String(wd.humid);
        pres_kpa = String(wd.pres_kpa);
        gas_kohms = String(wd.gas_kohms);
        wet = String(wd.wet_analog);
        light = String(wd.light_analog);
        String d = "T" + temp_c + "|H" + humid + "|P" + pres_kpa + "|G" + gas_kohms + "|R" + wet + "|L" + light;

#ifdef DEBUG
        Serial.print("Would transmit data: ");
        Serial.println(d);
#else
        ws_tx_data(2, d);
#endif

        ws_set_status_led(true);
        delay(30);
        ws_set_status_led(false);

        lastTransmit = millis();
    }

    delay(250);
}

/* ========================================================================== */
/*                        PRIVATE FUNCTION DEFINITIONS                        */
/* ========================================================================== */