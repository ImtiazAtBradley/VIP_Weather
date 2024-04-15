#pragma once

enum LightLevel
{
   SUNNY = 0,
   DARK = 1,
};

class WeatherData
{
 public:
   WeatherData(float temp_c = 0,
               float humid = 0,
               float pressure_kpa = 0,
               bool isRaining = 0,
               LightLevel lightLevel = LightLevel::SUNNY)
       : m_temp_c(temp_c), m_humid(humid), m_pressure_kpa(pressure_kpa), m_isRaining(isRaining),
         m_lightLevel(lightLevel)
   {
   }

   float m_temp_c;
   float m_humid;
   float m_pressure_kpa;
   bool m_isRaining;
   LightLevel m_lightLevel;

 private:
};
