/**
 * BRADLEY CAST PROTOCOL
 *
 * This header implementes the BradleyCast protocol for the weather station's
 * link between itself and a server running broker software.
 *
 * AUTHORS: ECE398-23SPR Weather Station Group
 *
 * CONTACT: jsimeone@mail.bradley.edu
 */

#ifndef __BRADLEY_CAST_DEFS_H__
#define __BRADLEY_CAST_DEFS_H__

#include <stdint.h>

/* ================================================== */
/*                   UNITS - SCALE                    */
/* ================================================== */

#define TEMPERATURE_SCALE_C (100)
#define HUMID_SCALE_PERCENT (100)
#define PRESSURE_SCALE_KPA  (100)

/* ================================================== */
/*                MESSAGE LEN (BTYES)                 */
/* ================================================== */

#define STATION_ID_LEN      (4)
#define MSG_ID_LEN          (2)
#define TEMP_LEN            (2)
#define HUMID_LEN           (2)
#define PRESSURE_LEN        (2)
#define RCSG_LEN            (1)

#define MSG_HEAD_LEN (STATION_ID_LEN + MSG_ID_LEN)

/* ================================================== */
/*                    ENUMERATIONS                    */
/* ================================================== */

typedef enum
{
   BC_SUNNY = 0,
   BC_CLOUDY = 1,
   BC_GLOOMY = 2
} bc_scg_t;

typedef enum
{
   MSG_ENV_ID = 0x0010,

   MSG_ID_INVALID,
} bc_msg_id_t;

/* ================================================== */
/*                       MESSAGES                     */
/* ================================================== */

/* MESSAGE STRUCTURE */

typedef struct
{
   uint32_t station_id;
   uint16_t msg_id;
   void *msg_paylod;
   size_t payload_len;
} bc_msg_t;

/* ENVIRONMENTAL DATA MESSAGE */

#define MSG_ENV_LEN         (7) /* Payload Length */
#define MSG_ENV_BITMSK_RAIN (0x01)
// Sunny/Cloudy/Gloomy
#define MSG_ENV_BITMSK_SCG  (0x06)

// Data that will be parsed into message
typedef struct
{
   float temp_c;
   float humid_prcnt;
   float press_kpa;
   // Rainy, sunny, cloudy, gloomy (see protocol documentation)
   uint8_t is_rainy;
   bc_scg_t light_level;
} bc_env_data_t;

#endif // __BRADLEY_CAST_DEFS_H__
