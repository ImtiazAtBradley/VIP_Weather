#pragma once

class WeatherData
{
 public:
   WeatherData(float temp_c = 0,
               float humid = 0,
               float pressure_kpa = 0,
               float gasKOhms = 0,
               int anRaining = 0,
               int anLightLevel = 0)
       : m_temp_c(temp_c), m_humid(humid), m_pressure_kpa(pressure_kpa), m_gasKOhms(gasKOhms), m_anRaining(anRaining),
         m_anLightLevel(anLightLevel)
   {
   }

   float m_temp_c;
   float m_humid;
   float m_pressure_kpa;
   float m_gasKOhms;
   int m_anRaining;
   int m_anLightLevel;

 private:
};
