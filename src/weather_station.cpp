/**
 * @file port.c
 * @author your name (you@domain.com)
 * @brief This is the file where we will define all of our functions from
 * weather_station.h
 *
 */
#include <Arduino.h>

#include <weather_station.h>

/* ========================================================================== */
/*                              PRIVATE FUNCTIONS                             */
/* ========================================================================== */

int hal_get_light_level() {
  /**
   * OWEN: Put your analog read functions here and return them. Use these
   * functions in your algorithm
   *
   * For example: `return analogRead(<YOUR PIN HERE>);`
   *
   */

  return -1;
}

int hal_get_water_level() {
  /**
   * Same as above, but with the water level
   */

  return -1;
}

/* ========================================================================== */
/*                                 PUBLIC API                                 */
/* ========================================================================== */

/**
 * \brief
 *
 * OWEN: These are the two functions you need to implement.
 * you'll need to identify analog values of both light and water to figure out
 * when it's raining, or when it's sunny. Feel free to pick BS values for now
 * until we can get some real data measuring when it's raining/not sunny/not.
 *
 * Let's make this really simple to start. For example:
 *
 * ```cpp
 * if (hal_get_light_level() > <SOME MADE UP VALUE>){
 *   return true;
 * } else {
 *   return false;
 * }
 * ```
 *
 */

/**
 * @brief Check if it is currently raining
 *
 * @return true -> Currently raining
 * @return false -> Not currently raining
 */
bool ws_is_raining() {}

/**
 * @brief Check if it is currently sunny out
 *
 * @return true -> Currently sunny
 * @return false -> Not currently sunny
 */
bool ws_is_sunny() {}