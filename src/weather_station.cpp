/**
 * @file port.c
 * @author your name (you@domain.com)
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

/* ========================================================================== */
/*                              STATIC VARIABLES                              */
/* ========================================================================== */

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

static void priv_handle_captive_api();

static void priv_redirect();

static void priv_handle_success();

/* ========================================================================== */
/*                                 PUBLIC API                                 */
/* ========================================================================== */

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
    delay(100);

    return rc;
}

bool
ws_start_http_server() {
    // Responds to all DNS requests with localIp, creating a captive portal
    dnsServer.start(53, "beccweather.com", localIp);

    server.on("/", priv_handle_root);
    server.on("/captive-portal/api", priv_handle_captive_api);
    // Credit to:
    // https://github.com/HerrRiebmann/Caravan_Leveler/blob/main/Webserver.ino for
    // captive portal redirects
    // Allways redirect to captive portal. Request comes with IP (8.8.8.8) or URL
    // (connectivitycheck.XXX / captive.apple / etc.)
    server.on("/generate_204", priv_redirect); // Android captive portal.
    server.on("/fwlink", priv_redirect);       // Microsoft captive portal.

    server.on("/connecttest.txt", priv_redirect);     // www.msftconnecttest.com
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
    char buf[1024] = {0};

    snprintf(buf, sizeof(buf),
             "<html lang=\"en\">\n"
             "<head>\n"
             "<meta charset=\"UTF-8\">\n"
             "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
             "<title>Weather Station</title>\n"
             "<style>\n"
             "table {\n"
             "font-family: arial, sans-serif;\n"
             "border-collapse: collapse;\n"
             "width: 100%%;\n"
             "}\n"
             "\n"
             "td, th {\n"
             "border: 1px solid #dddddd;\n"
             "text-align: left;\n"
             "padding: 8px;\n"
             "}\n"
             "\n"
             "tr:nth-child(even) {\n"
             "background-color: #dddddd;\n"
             "}\n"
             "</style>\n"
             "</head>\n"
             "<body>\n"
             "<table>\n"
             "<tr>\n"
             "<th>Degrees C</th>\n"
             "<th>Humid</th>\n"
             "<th>Pressure</th>\n"
             "<th>Is Sunny</th>\n"
             "<th>Is Raining</th>\n"
             "</tr>\n"
             "<tr>\n"
             "<td>%.2f</td>\n"
             "<td>%.2f</td>\n"
             "<td>%.2f</td>\n"
             "<td>%u</td>\n"
             "<td>%u</td>\n"
             "</tr>\n"
             "</table> \n"
             "</body>\n"
             "</html>",
             webWeatherData.temp_c, webWeatherData.humid, webWeatherData.pres_kpa, (uint8_t)webWeatherData.isSunny,
             (uint8_t)webWeatherData.isRaining);

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
                  "  \t<!-- Kindly remove yourself from my source \\- The BECC Weather "
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
priv_handle_captive_api() {}

void
priv_redirect() {
    server.sendHeader("Location", String("http://") + localIp.toString(), true);
    server.send(302, "text/plain",
                "");        // Empty content inhibits Content-length header so we have to
                            // close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
}

void
priv_handle_success() {
    server.send(200, "text/plain", "success");
}