#include <MessageParse.h>
#include "MessageResponse.h"
#include "WeatherData.h"
#include <algorithm>
#include <string>
#include <vector>
#include <optional>
#include <iostream>

std::optional<MessageResponse>
MessageParse::parseMessage(const std::string &radioMessage)
{
   WeatherData d = WeatherData();
   std::vector<std::string> tokens = std::vector<std::string>();
   std::vector<std::string> weatherTokens = std::vector<std::string>();
   std::string inMessage = radioMessage;
   std::string data;
   size_t pos = 0;
   int id, rssi, snr;

   tokens.reserve(5);

   while ((pos = inMessage.find(",")) != std::string::npos)
   {
      tokens.push_back(inMessage.substr(0, pos));
      inMessage.erase(0, pos + 1);
   }
   tokens.push_back(inMessage);

   // Should have 5 elements
   if (tokens.size() != 5)
   {
      return {};
   }

   data = tokens[2];

   // PARSE DATA FROM PAYLOAD
   while ((pos = data.find("|")) != std::string::npos)
   {
      weatherTokens.push_back(data.substr(0, pos));
      data.erase(0, pos + 1);
   }
   weatherTokens.push_back(data);

   try
   {

      // Parse out ID

      id = std::stoi(tokens[0].substr(tokens[0].find("=") + 1, std::string::npos));

      // Parse out RSSI

      rssi = std::stoi(tokens[3]);

      // Parse out SNR

      snr = std::stoi(tokens[4]);

      // Parse out weather data

      for (std::string s : weatherTokens)
      {
         std::string key = s.substr(0, 1);
         std::string value = s.erase(0, 1);

         if (key == "T")
         {
            d.m_temp_c = std::stof(value);
         }
         else if (key == "H")
         {
            d.m_humid = std::stof(value);
         }
         else if (key == "P")
         {
            d.m_pressure_kpa = std::stof(value);
         }
         else if (key == "G")
         {
            d.m_gasKOhms = std::stof(value);
         }
         else if (key == "R")
         {
            d.m_anRaining = std::stoi(value);
         }
         else if (key == "L")
         {
            d.m_anLightLevel = std::stoi(value);
         }
      }
   }
   catch (std::invalid_argument &e)
   {
      std::cerr << "[ERROR] Failed to parse packet\n";
      return {};
   }

   return MessageResponse(id, rssi, snr, d);
}
