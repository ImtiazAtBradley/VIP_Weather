#include <MessageParse.h>
#include "WeatherData.h"
#include <algorithm>
#include <string>
#include <vector>
#include <optional>

std::optional<WeatherData> MessageParse::parseMessage(const std::string &radioMessage) {
   std::string inMessage = radioMessage;
   std::string data;

   size_t pos = 0;

   std::vector<std::string> tokens = std::vector<std::string>();
   std::vector<std::string> weatherTokens = std::vector<std::string>();
   tokens.reserve(5);

   while ((pos = inMessage.find(",")) != std::string::npos){
      tokens.push_back(inMessage.substr(0, pos));
      inMessage.erase(0, pos + 1);
   }
   tokens.push_back(inMessage);

   // Should have 5 elements
   if (tokens.size() != 5){
      return {};
   }

   data = tokens[2];

   // PARSE DATA FROM PAYLOAD
   while ((pos = data.find("|")) != std::string::npos){
      weatherTokens.push_back(data.substr(0, pos));
      data.erase(0, pos + 1);
   }
   weatherTokens.push_back(data);

   WeatherData d = WeatherData();

   for (std::string s : weatherTokens) {
      std::string key = s.substr(0, 1);
      std::string value = s.erase(0, 1);


      if (key == "T"){
         d.m_temp_c = std::stof(value);
      } else if (key == "H"){
         d.m_humid = std::stof(value);
      } else if (key == "P") {
         d.m_pressure_kpa = std::stof(value);
      } else if (key == "R") {
         d.m_isRaining = s != "0";
      } else if (key == "L") {
         d.m_lightLevel = LightLevel(std::stoi(value));
      }
   }

   return d;
}
