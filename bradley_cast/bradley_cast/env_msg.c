#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bradley_cast.h"

#if defined(__linux__)
#include "arpa/inet.h"
#elif defined(__WIN32__) || defined(_WIN32)
#include <winsock.h>
#else
#error "Neither linux or windows compilation detected"
#endif

#include <winsock.h>
#include "bradley_cast_defs.h"

static int
get_env_payload(const bc_env_data_t *const pEnvData, uint8_t (*payloadBuffer)[MSG_ENV_LEN])
{
   // Assume all pointers are safe as this is a function internal to env_msg.c
   uint8_t rscgBuf;

   *(uint16_t *)(*payloadBuffer + 0) = htons((uint16_t)(pEnvData->temp_c * TEMPERATURE_SCALE_C));
   *(uint16_t *)(*payloadBuffer + 2) = htons((uint16_t)(pEnvData->humid_prcnt * HUMID_SCALE_PERCENT));
   *(uint16_t *)(*payloadBuffer + 4) = htons((uint16_t)(pEnvData->press_kpa * PRESSURE_SCALE_KPA));
   // Zero buffer first
   *(uint8_t *)(*payloadBuffer + 6) = 0;
   *(uint8_t *)(*payloadBuffer + 6) = (pEnvData->light_level << 1) | (pEnvData->is_rainy ? 1 : 0);

   return 0;
}

static int
parse_env_payload(bc_env_data_t *const pEnvData, uint8_t *payloadBuffer, size_t len)
{
   if (len != MSG_ENV_LEN){
      return -1;
   }

   pEnvData->temp_c = (float)ntohs(*((uint16_t *)(payloadBuffer + 0))) / TEMPERATURE_SCALE_C;
   pEnvData->humid_prcnt = (float)ntohs(*((uint16_t *)(payloadBuffer + 2))) / HUMID_SCALE_PERCENT;
   pEnvData->press_kpa = (float)ntohs(*((uint16_t *)(payloadBuffer + 4))) / PRESSURE_SCALE_KPA;

   pEnvData->is_rainy = *((uint16_t *)(payloadBuffer + 6)) & MSG_ENV_BITMSK_RAIN;
   pEnvData->light_level = (*((uint16_t *)(payloadBuffer + 6)) & MSG_ENV_BITMSK_SCG) >> 1;

   return 0;
}

int
bc_build_env_msg(const uint32_t stationId, bc_env_data_t *const pEnvData, bc_msg_t *const pMsg)
{

   uint8_t payloadBuf[MSG_ENV_LEN] = {0};

   if (pEnvData == NULL || pMsg == NULL)
   {
      // Failure by null pointer
      return -1;
   }

   if (get_env_payload(pEnvData, &payloadBuf) != 0)
   {
      // Failure by fail to parse environment payload
      return -1;
   }

   if (bc_msg_init(stationId, MSG_ENV_ID, MSG_ENV_LEN, pMsg) != 0){
      // Failure by init function fail
      return -1;
   }

   // Copy data into payload buffer of message
   memcpy(pMsg->msg_paylod, payloadBuf, pMsg->payload_len);

   return 0;
}

// TODO: Move bc_msg_t specific parsing to a function in bradley_cast.c, this function should only be responsible
//   for parsing the payload of the enviornment message.
int
bc_parse_env_msg(bc_env_data_t *const pEnvData, bc_msg_t *const pMsg)
{

   if (pEnvData == NULL || pMsg == NULL || pMsg->msg_paylod == NULL)
   {
      // Failure by null pointer
      return -1;
   }

   if (pMsg->payload_len != MSG_ENV_LEN){
      // Failure by invalid payload length
      return -1;
   }

   // This assumption can be made because we checked size earlier
   if (parse_env_payload(pEnvData, pMsg->msg_paylod, pMsg->payload_len) != 0){
      // Failure by parse failure
      return -1;
   }

   return 0;
}
