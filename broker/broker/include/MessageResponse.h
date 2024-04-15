#pragma once

#include "WeatherData.h"

class MessageResponse
{
 public:
   MessageResponse(const int id, const int rssi, const int snr, const WeatherData &data)
       : m_id(id), m_rssi(rssi), m_snr(snr), m_data(data)
   {
   }

   int m_id;
   int m_rssi;
   int m_snr;
   WeatherData m_data;
};
