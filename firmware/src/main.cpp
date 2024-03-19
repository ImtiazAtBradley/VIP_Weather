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

void
setup() {
    Serial.begin(115200);
    ws_start_soft_ap();
    ws_start_http_server();

    if (!ws_init_sensors()) {
        Serial.println("Failed to initialize sensors");
        while (1)
            ;
    } else {
        Serial.println("Successfully Initialized Sensors");
    }

    Serial.println("Server ready");
}

void
loop() {
    ws_server_task();

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

    ws_website_set_data(&wd);

    if (millis() - lastRead > 500) {
        Serial.print("Temperature....");
        Serial.println(temp_c);

        Serial.print("Pressure.......");
        Serial.println(pres_kpa);

        Serial.print("Humidity.......");
        Serial.println(humidity);

        Serial.print("Water Level....");
        Serial.println(analogRead(33));

        Serial.print("Light Level....");
        Serial.println(analogRead(32));

        lastRead = millis();
    }

    delay(15);
}

/* ========================================================================== */
/*                        PRIVATE FUNCTION DEFINITIONS                        */
/* ========================================================================== */