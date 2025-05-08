// System Includes
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

// Libs
#include "curl/curl.h"

// Internal Libs
#include "broker.h"
#include "MessageParse.h"
#include "MessageResponse.h"
#include "WeatherData.h"
#include "constants.h"
#include "utils.h"

// Definitions

#define MAX_JSON_SIZE   (250) // Should be more than enough for weather data - adjust as needed
#define MAX_HEAD_LEN    (50)  // Maximum HTTP header len for this application
#define MAX_LIGHT_LEVEL (10)  // Maximum length of light level string

Broker::Broker(std::chrono::milliseconds schedulerTimeMs, std::string path, std::string url, std::string key, std::filesystem::path logFilePath, int logFileSize, int numLogFiles)
    : m_url(url), m_apiKey(key), m_frequency_ms(schedulerTimeMs), m_filePath(path), m_wsLog{logFilePath, logFileSize, numLogFiles}
{
   // TODO Add logging here
}

bool
Broker::serialFileGood()
{
   return FileExists(m_filePath.c_str());
}

bool
Broker::runScheduler()
{
   auto now = std::chrono::high_resolution_clock::now();

   if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastRunTime_ms) > m_frequency_ms)
   {
      // Run internal tasks
      runTasks();
      m_lastRunTime_ms = std::chrono::high_resolution_clock::now();
      return true; // Ran tasks
   }

   return false;
}

bool
Broker::setupSerialPort()
{
   int file, rc;
   struct termios options;

   file = open(m_filePath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
   if (file < 0)
   {
      return false;
   }
   m_fd = file;

   // Set baud rate to 115200
   cfsetispeed(&options, B115200);
   cfsetospeed(&options, B115200);

   // Input settings
   options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IXON | IXOFF | IXANY);
   options.c_oflag &= ~OPOST;
   options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
   options.c_cflag &= ~(CSIZE | PARENB);
   options.c_cflag |= (CS8 | CLOCAL | CREAD);

   options.c_cc[VMIN] = 0;
   options.c_cc[VTIME] = 0;

   rc = tcsetattr(file, TCSANOW, &options);

   if (rc == 0)
   {
      // Flush the current buffer of data
      // See: https://stackoverflow.com/questions/13013387/clearing-the-serial-ports-buffer
      sleep(2);
      tcflush(m_fd, TCIOFLUSH);
      m_serialUp = true;
      return true;
   }
   else
   {
      return false;
   }
}

bool
Broker::writeToPort(const std::string &val)
{
   if (!m_serialUp)
   {
      if (!setupSerialPort())
      {
         return false;
      }
   }

   if (val.size() <= 0)
   {
      return false;
   }

   size_t txNum = write(m_fd, val.c_str(), val.size());

   return txNum == val.size();
}

std::string
Broker::readFromPort()
{
   char buf[128] = {0};

   if (!m_serialUp)
   {
      if (!setupSerialPort())
      {
         return "";
      }
   }

   ssize_t rxNum = read(m_fd, buf, sizeof(buf));

   return std::string(buf, strlen(buf));
}

// ============================== PRIVATE FUNCTIONS ==============================

void
Broker::runTasks()
{
   // Try to receive from buffer, and print to standard out
   std::string rcv = readFromPort();

   if (rcv.size() > 0)
   {
      std::cout << "[DEBUG] Raw from radio:" << rcv;
      std::optional<MessageResponse> response = MessageParse::parseMessage(rcv);
      if (response)
      {
         std::cout << "[DEBUG (" << GetUnixTimestamp() << ")] Parsed weather data: T:" << response->m_data.m_temp_c
                   << " H:" << response->m_data.m_humid << " P:" << response->m_data.m_pressure_kpa
                   << "G:" << response->m_data.m_gasKOhms << " R:" << response->m_data.m_anRaining
                   << " L:" << response->m_data.m_anLightLevel << "\n";
         std::cout << "Network Data ID: " << response->m_id << " RSSI:" << response->m_rssi
                   << " SNR:" << response->m_snr << "\n";

         if (!Broker::PostToAPI(response->m_data, m_url, m_apiKey))
         {
            std::cerr << "[ERROR] Failed to post data to API\n";
         }
      }
   }
}

Broker::~Broker()
{
   // Close file
   close(m_fd);
}

// ============================== STATIC FUNCTIONS ===============================

// Yes, a bit of a C/CPP mix, as the CURL code was originally written in C
bool
Broker::PostToAPI(WeatherData data, std::string url, std::string apiKey)
{
   CURL *curl;
   CURLcode res;
   // Data structure to hold headers
   struct curl_slist *hs = NULL;
   char authorization[MAX_HEAD_LEN] = {0};
   char jsonData[MAX_JSON_SIZE] = {0};

   snprintf(jsonData,
            MAX_JSON_SIZE,
            "{"
            "\"timestamp_unix_ms\":%ld,"
            "\"temp_c\":%0.2f,"
            "\"humid_prcnt\":%0.2f,"
            "\"pressure_kpa\":%0.2f,"
            "\"gas_kohms\":%0.2f,"
            "\"rain_an\":%d,"
            "\"light_an\":\"%d\""
            "}",
            GetUnixTimestamp(),
            data.m_temp_c,
            data.m_humid,
            data.m_pressure_kpa,
            data.m_gasKOhms,
            data.m_anRaining,
            data.m_anLightLevel);

   if (strlen(jsonData) > MAX_JSON_SIZE)
   {
      // JSON data is too big - for whatever reason
      fprintf(stderr, "JSON data is too long\n");
      return -1;
   }

   // Initialize CURL
   res = curl_global_init(CURL_GLOBAL_ALL);
   if (res != CURLE_OK)
   {
      fprintf(stderr, "Failed to initialize libcurl \"%s\" [%d]\n", curl_easy_strerror(res), res);
      return 1;
   }

   // Initialize CURL - pt 2
   curl = curl_easy_init();
   if (curl == NULL)
   {
      fprintf(stderr, "Failed to get CURL handle\n");
      return 1;
   }

   // Set up authorization header
   snprintf(authorization, MAX_HEAD_LEN, "Authorization: %s", apiKey.c_str());

   // Set our headers - content-type and authorization
   hs = curl_slist_append(hs, "Content-Type: application/json");
   hs = curl_slist_append(hs, authorization);
   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);

   // Set the URL and the post data
   curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

   // Set post data - JSON would go here, describing weather data
   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData);

   res = curl_easy_perform(curl);
   if (res != CURLE_OK)
   {
      fprintf(stderr, "Failed to perform HTTP POST transfer: \"%s\" [%d]\n", curl_easy_strerror(res), res);
      return false;
   }

   curl_easy_cleanup(curl);
   curl_global_cleanup();

   return true;
}

void
Broker::help()
{
   std::cout << "\nBRADLEY CAST BROKER - ACCESS POINT OF WEATHER STATIONS TO "
                "SERVER DATABASE\n\n"
             << "AUTHORS:\n  BRADLEY UNIVERSITY ECE398 WEATHER STATION PROJECT GROUP 2024\n"
             << "USAGE: \n  bradley-cast-broker [serial dev file] [API URL] [API key file]\n";
}

void
Broker::printProgramHeader()
{
   std::cout << "BRADLEY CAST BROKER V" << bc_broker::version::major << "." << bc_broker::version::minor << "."
             << bc_broker::version::patch << "\n\n";
}
