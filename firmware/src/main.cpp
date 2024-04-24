/**
 * @file main.cpp
 * @brief VIP Project weather station source
 * 
 */
#include <Arduino.h>
#include <weather_station.h>

// #define TX_INTERVAL_MS (1800000)
// FOR DEMO ONLY
#define TX_INTERVAL_MS (30000)
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

static float temp_c = 0.0f, humidity = 0.0f, pres_kpa = 0.0f;
bool isRaining = false, isSunny = false;

static weather_data_t wd = {0};

static long lastBlink = LONG_MAX;
static long lastTransmit = LONG_MAX;

void
setup() {
    Serial.begin(115200);

    Serial.println("BOOTING...");

    delay(1000);

    while (!ws_init()) {
        // Blink a couple times to notify user
        for (int i = 0; i < 3; ++i) {
            ws_set_status_led(true);
            delay(1000);
            ws_set_status_led(false);
            delay(1000);
        }
    }

    Serial.println("BOOT OK");
}

void
loop() {
    temp_c = ws_get_temperature();
    isRaining = ws_is_raining();
    humidity = ws_get_humidity();
    pres_kpa = ws_get_pressure();
    isSunny = ws_is_sunny();

    wd.temp_c = temp_c;
    wd.isRaining = isRaining;
    wd.humid = humidity;
    wd.pres_kpa = pres_kpa;
    wd.isSunny = isSunny;

    // TODO: UNCOMMENT AFTER DEMO
    if (millis() - lastBlink > 15000 && false) {
        // Blink to indicate it's not dead
        ws_set_status_led(true);
        delay(20);
        ws_set_status_led(false);

#ifdef DEBUG
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

        Serial.print("........................................\n");
#endif

        lastBlink = millis();
    }

    if (millis() - lastTransmit > TX_INTERVAL_MS) {

        String temp_c, humid, pres_kpa, isRaining, lightLevel;
        temp_c = String(wd.temp_c);
        humid = String(wd.humid);
        pres_kpa = String(wd.pres_kpa);
        isRaining = String(wd.isRaining);
        lightLevel = String(wd.isSunny);
        String d = "T" + temp_c + "|H" + humid + "|P" + pres_kpa + "|R" + isRaining + "|L" + lightLevel;

        ws_tx_data(1, d);

        ws_set_status_led(true);
        delay(500);
        ws_set_status_led(false);

        lastTransmit = millis();
    }

    delay(15);
}

/* ========================================================================== */
/*                        PRIVATE FUNCTION DEFINITIONS                        */
/* ========================================================================== */