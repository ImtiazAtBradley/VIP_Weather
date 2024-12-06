/**
 * @file main.cpp
 * @brief VIP Project weather station source
 * 
 */
#include <Arduino.h>
#include <weather_station.h>

#define TX_INTERVAL_MS (1800000)
// #define DEBUG

/* ========================================================================== */
/*                        PRIVATE FUNCTION DECLARATIONS                       */
/* ========================================================================== */

/* ========================================================================== */
/*                              STATIC VARIABLES                              */
/* ========================================================================== */

/* ========================================================================== */
/*                                MAIN PROGRAM                                */
/* ========================================================================== */

static float temp_c = 0.0f, humidity = 0.0f, pres_kpa = 0.0f, gas_kohms = 0.0f;
bool isRaining = false, isSunny = false;

static weather_data_t wd = {0};

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
    if(!ws_init()) {
        // Blink a couple times to notify user
        for (int i = 0; i < 3; ++i) {
            ws_set_status_led(false);
            delay(1000);
            ws_set_status_led(true);
            delay(1000);
        }

        Serial.println("BOOT RTRY");
        // Wait before soft restart
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
    temp_c = ws_get_temperature();
    isRaining = ws_is_raining();
    humidity = ws_get_humidity();
    pres_kpa = ws_get_pressure();
    gas_kohms = ws_get_gas_sensor();
    isSunny = ws_is_sunny();

    wd.temp_c = temp_c;
    wd.isRaining = isRaining;
    wd.humid = humidity;
    wd.pres_kpa = pres_kpa;
    wd.gas_kohms = gas_kohms;
    wd.isSunny = isSunny;


#ifdef DEBUG
    if (millis() - lastDebug > 15000) {
        // Blink to indicate it's not dead
        ws_set_status_led(true);
        delay(20);
        ws_set_status_led(false);
        // Print out some debug stuff
        Serial.println("[DEBUG] CURRENT VALUES: ");
        Serial.print("Water Level..........");
        Serial.print(ws_raw_raining());
        Serial.print("\n");
        Serial.print("Light Level..........");
        Serial.print(ws_raw_light());
        Serial.print("\n");
        Serial.print("Pressure Level.......");
        Serial.print(ws_get_pressure());
        Serial.print("\n");
        Serial.print("Humidity Level.......");
        Serial.print(ws_get_humidity());
        Serial.print("\n");
        Serial.print("Temperature..........");
        Serial.print(ws_get_temperature());
        Serial.print("\n");
        Serial.print("Gas Sensor...........");
        Serial.print(ws_get_gas_sensor());
        Serial.print("\n");
        

        Serial.print("........................................\n");

        lastDebug = millis();
    }
#endif

    if (millis() - lastTransmit > TX_INTERVAL_MS) {

        String temp_c, humid, pres_kpa, isRaining, lightLevel, gas_kohms;
        temp_c = String(wd.temp_c);
        humid = String(wd.humid);
        pres_kpa = String(wd.pres_kpa);
        gas_kohms = String(wd.gas_kohms);
        isRaining = String(wd.isRaining);
        lightLevel = String(wd.isSunny);
        String d = "T" + temp_c + "|H" + humid + "|P" + pres_kpa + "|G" + gas_kohms + "|R" + isRaining + "|L" + lightLevel;

        #ifdef DEBUG
        Serial.println("Transmitting data...");
        #endif
        ws_tx_data(1, d);

        ws_set_status_led(true);
        delay(30);
        ws_set_status_led(false);

        lastTransmit = millis();
    }

    delay(15);
}

/* ========================================================================== */
/*                        PRIVATE FUNCTION DEFINITIONS                        */
/* ========================================================================== */