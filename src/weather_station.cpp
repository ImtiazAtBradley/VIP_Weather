/**
 * @file port.c
 * @author Owen T. Joseph ojoseph@mail.bradley.edu
 * @brief This is the file where we will define all of our functions from
 * weather_station.h
 **/

#include <Arduino.h>
#include <weather_station.h>

/* ========================================================================== */
/*                              PRIVATE FUNCTIONS                             */
/* ========================================================================== */

int hal_get_light_level() {
  //lightPin = 26 max: 4095, min: 0
  return analogRead(26);
}

int hal_get_water_level() {
  //waterPin = 25 max: ?,    min: 0
  return analogRead(25);
}

/* ========================================================================== */
/*                                 PUBLIC API                                 */
/* ========================================================================== */

bool ws_is_raining() {
  if (hal_get_light_level() > 0)
    return true;
  else
    return false;
}

bool ws_is_sunny() {
  if (hal_get_light_level() > 450)
    return true;
  else
    return false;
}