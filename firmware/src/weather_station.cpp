/**
 * @file port.c
 * @author Owen J., Jacob S.
 * @brief This is the file where we will define all of our functions from
 * weather_station.h
 *
 */

#include <secrets.h>
#include <weather_station.h>
#include <Arduino.h>
// Very very crude implementations of WiFi & HTTP server functionality
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <weather_station.h>

/* ========================================================================== */
/*                              STATIC VARIABLES                              */
/* ========================================================================== */

// BME280
Adafruit_BME280 bme; // I2C

// Ip Addresses
IPAddress localIp(192, 168, 1, 1);
IPAddress gatewayIp(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Instantiate a web server on port 80
WebServer server(80);

// DNS Server that allows us to have captive portal
DNSServer dnsServer;

weather_data_t webWeatherData = {0};

/* ========================================================================== */
/*                              PRIVATE FUNCTIONS                             */
/* ========================================================================== */

static String priv_get_root_page();

static String priv_get_404_page();

static void priv_handle_root();

static void priv_handle_404();

static String priv_get_api_env_data();

static void priv_redirect();

static void priv_handle_success();

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
ws_init_sensors() {

    Wire.begin(21, 22, 5000);

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

void
ws_website_set_data(const weather_data_t* pData) {
    webWeatherData = *pData;
}

bool
ws_start_soft_ap() {
    bool rc = true;
    rc &= WiFi.mode(WIFI_AP);
    rc &= WiFi.softAP(WS_SSID, WS_PASS);
    rc &= WiFi.softAPConfig(localIp, gatewayIp, subnet);
    rc &= WiFi.setTxPower(WIFI_POWER_19_5dBm);
    delay(100);

    return rc;
}

bool
ws_start_http_server() {
    // Responds to all DNS requests with localIp, creating a captive portal
    dnsServer.start(53, "*", localIp);
    server.on("/", priv_get_api_env_data);
    //server.on("/", priv_handle_root);
    //server.on("/api/envData", priv_get_api_env_data);
    // Credit to:
    // https://github.com/HerrRiebmann/Caravan_Leveler/blob/main/Webserver.ino for
    // captive portal redirects
    // Allways redirect to captive portal. Request comes with IP (8.8.8.8) or URL
    // (connectivitycheck.XXX / captive.apple / etc.)
    server.on("/generate_204", priv_redirect);        // Android captive portal.
    server.on("/fwlink", priv_redirect);              // Microsoft captive portal.

    server.on("/connecttest.txt", priv_redirect);     // www.msftconnecttest.com
    server.on("/redirect", priv_redirect);            // www.msftconnecttest.com (redirect)
    server.on("/hotspot-detect.html", priv_redirect); // captive.apple.com

    server.on("/success.txt",
              priv_handle_success); // detectportal.firefox.com/sucess.txt

    server.onNotFound(priv_handle_404);

    server.begin();
    // Right now, none of the functions have return codes
    return true;
}

void
ws_server_task() {
    dnsServer.processNextRequest();
    server.handleClient();
}

/* ========================================================================== */
/*                        PRIVATE FUNCTION DEFINITIONS                        */
/* ========================================================================== */

String
priv_get_root_page() {

    // Temporary buffer to store string
    char buf[2848] = {0};

    snprintf(
        buf, sizeof(buf),
        // Minified website 2024-02-29 (Take out all whitespace)
        "<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" "
        "content=\"width=device-width,initial-scale=1\"><title>Weather "
        "Station</"
        "title><style>html{font-family:Verdana,Geneva,Tahoma,sans-serif}table{font-family:arial,sans-serif;border-"
        "collapse:collapse;width:100%%}td,th{border:1px solid "
        "#ddd;text-align:left;padding:8px}tr:nth-child(even){background-color:#ddd}caption{caption-side:bottom;"
        "text-align:start;padding-top:8px;color:#27272793}.table-container{border:5px solid "
        "#ff5d5d}.container{margin:auto;width:80%%;padding:10px}</style></head><body><div class=\"container "
        "table-container\"><h1 style=\"text-align:center\">BECC Weather Station</h1><table><tr><th>Degrees "
        "F</th><th>Humid</th><th>Pressure</th></tr><tr><td>%.2f</td><td>%.2f</td><td>%.2f</td></tr></"
        "table><table><tr><th>Is Sunny</th><th>Is "
        "Raining</th></tr><tr><td>%u</td><td>%u</td></tr><caption>NOTE: "
        "This device is under testing, and is currently displaying environment information of the:<b>controls "
        "lab</b></caption></table></div><div class=\"container\"><hr><h2>What am "
        "I?</h2><p>Currently, you're "
        "looking at a web server run on an ESP32 micro-controller, which you connected to with your device. Right "
        "now, <strong>I'm under development by my creators, and just sitting on a table in a lab "
        "somewhere.</strong></p><h2>How Do "
        "I Work?</h2><p>I have a couple sensors that are detecting things about my environment:</p><ul><li>DHT11 "
        "- temp and humidity (eventually will be a bme680)</li><li>Generic Arduino Water Level Sensor - For "
        "detecting rain</li><li>Photo-resistor - For detecting if it's light out or not</li></ul><p>My brain is an "
        "ESP32 micro-controller, but hopefully in the future I'll support talking to a higher-order system like a "
        "linux computer, or maybe an HTTP API.</p><h2>Will I Actually Show the Weather For Real?</h2><p>At some "
        "point! Barring the apocalypse, of course. Currently, I'm on schedule to be up and running showing you "
        "the weather by the end of the semester. (Hopefully, in a far prettier manner, or even sooner than the "
        "end of the semester)</p><h2>Why No Internet When You're Connected To Me?</h2><p>I'm hosting my own, "
        "private, isolated network. There isn't any access to the outside world, or other devices on this "
        "network. Just some weather!</p><h2>Who Made Me?</h2><p>I was designed as a project in the EE department "
        "by students in the ECE-398 project class. In time, I'll be an easy and convenient way for students to "
        "access the Bradley University campus weather.</p><p>My current human overlords are:</p><ul><li>Kyle "
        "K.</li><li>Chris H.</li><li>Emily A.</li><li>Owen J.</li><li>Samantha P.</li><li>Jacob "
        "S.</li></ul><h2>Contact</h2><p>Comments? Questions? Concerns? Contact:<br></p><table><tr><td>Jacob S. "
        "(ECE 398 Student)</td><td>jsimeone@mail.bradley.edu</td></tr><tr><td>Dr. Imtiaz (ECE 398 "
        "Professor)</td><td>mimtiaz@fsmail.bradley.edu</td></tr></table></div></body></html>",
        webWeatherData.temp_c * (1.8f) + 32, webWeatherData.humid, webWeatherData.pres_kpa,
        (uint8_t)webWeatherData.isSunny, (uint8_t)webWeatherData.isRaining);

    return String(buf);
}

String
priv_get_api_env_data()
{
    char buf[512] = {0};

    snprintf(
        buf,
        sizeof(buf),
        "{ \"humid_percent\" : %.2f, \"temp_c\" : %.2f, \"press_kpa\" : %.2f, \"isRaining\" : %u, \"isSunny\" : %u}",
        webWeatherData.humid,
        webWeatherData.temp_c,
        webWeatherData.pres_kpa,
        webWeatherData.isRaining,
        webWeatherData.isSunny
    );

    return String(buf);
}

String
priv_get_404_page() {
    return String("<!DOCTYPE html>\n"
                  "<html lang=\"en\">\n"
                  "  <head>\n"
                  "    <meta charset=\"UTF-8\">\n"
                  "    <meta name=\"viewport\" content=\"width=device-width, "
                  "initial-scale=1.0\">\n"
                  "    <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\n"
                  "    <title>BECC Weather Station</title>\n"
                  "  </head>\n"
                  "  <body>\n"
                  "  \t<!-- Kindly remove yourself from my source - The BECC Weather "
                  "Station -->\n"
                  "        <h1>You're Not Supposed To Be Here >:(<h1>\n"
                  "  </body>\n"
                  "</html>\n");
}

void
priv_handle_root() {
    server.send(200, "text/html", priv_get_root_page());
}

void
priv_handle_404() {
    server.send(404, "text/html", priv_get_404_page());
}

void
priv_redirect() {
    server.sendHeader("Location", String("http://") + localIp.toString() + "/", true);
    server.send(302, "text/plain",
                "");        // Empty content inhibits Content-length header so we have to
                            // close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
}

void
priv_handle_success() {
    server.send(200, "text/plain", "success");
}
