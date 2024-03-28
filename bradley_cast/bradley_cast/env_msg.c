#include <stdlib.h>
#include <string.h>

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

int
bc_build_env_msg(const uint64_t stationId, bc_env_data_t *const pEnvData, bc_msg_t *const pMsg)
{

   uint8_t payloadBuf[MSG_ENV_LEN] = {0};
   void *buf;

   if (pEnvData == NULL || pMsg == NULL)
   {
      // Failure by null pointer
      return -1;
   }

   // Allocate memory for payload. THIS NEEDS TO BE FREED LATER
   // WITH bc_free_msg_payload()
   if ((buf = malloc(MSG_ENV_LEN)) == NULL)
   {
      // Fail by no mem
      return -1;
   }

   if (get_env_payload(pEnvData, &payloadBuf) != 0)
   {
      // Failure by fail to parse environment payload
      return -1;
   }

   pMsg->station_id = stationId;
   pMsg->msg_id = MSG_ENV_ID;
   pMsg->payload_len = MSG_ENV_LEN;
   // Point to newly allocated buffer to store payload in
   pMsg->msg_paylod = buf;

   // NOTE: Some systems provie safer versions of memcpy(), but for portability,
   //   just use it directly from standard string.h
   memcpy(pMsg->msg_paylod, &payloadBuf, MSG_ENV_LEN);

   return 0;
}
