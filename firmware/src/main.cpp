/**
 * @file main.cpp
 * @brief VIP Project weather station source
 * 
 */
#include <Arduino.h>
#include <weather_station.h>

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

static long lastRead = 0;
static long lastTransmit = 0;

void
setup() {
    Serial.begin(115200);

    Serial.println("BOOTING...");

    if (!ws_init_sensors()) {
        Serial.println("Failed to initialize sensors");
        while (1)
            ;
    } else {
        Serial.println("Successfully Initialized Sensors");
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

    if (millis() - lastTransmit > 10000) {

        // CODE HERE
        String temp_c, humid, press_kpa, isRaining, lightLevel;
        temp_c = String(wd.temp_c);
        humid = String(wd.humid);
        press_kpa = String(wd.pres_kpa);
        isRaining = String(wd.isRaining);
        lightLevel = String(wd.isSunny);
        String d = "T" + temp_c + "|H" + humid + "|P" + press_kpa + "|R" + isRaining + "|L" + lightLevel;

        ws_tx_data(1, d);

        lastTransmit = millis();
    }

    if(millis() - lastTransmit > 10000) {

        String temp_c, humid, pres_kpa, isRaining, lightLevel;
        temp_c = String(wd.temp_c);
        humid = String(wd.humid);
        pres_kpa = String(wd.pres_kpa);
        isRaining = String(wd.isRaining);
        lightLevel = String(wd.isSunny);
        String d = "T" + temp_c + "|H" + humid + "|P" + pres_kpa + "|R" + isRaining + "|L" + lightLevel;

        ws_tx_data(1, d);

        lastTransmit = millis();
    }

    delay(15);
}

/* ========================================================================== */
/*                        PRIVATE FUNCTION DEFINITIONS                        */
/* ========================================================================== */