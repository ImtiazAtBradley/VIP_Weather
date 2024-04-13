#include "bradley_cast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bradley_cast_defs.h"
#if defined(__linux__)
#include "arpa/inet.h"
#elif defined(__WIN32__) || defined(_WIN32)
#include <winsock.h>
#else
#error "Neither linux or windows compilation detected"
#endif

int
bc_free_msg_payload(bc_msg_t *const pMsg)
{
   if (pMsg->msg_paylod)
   {
      free(pMsg->msg_paylod);
   }

   return 0;
}

int
bc_is_msg_id_ok(const bc_msg_id_t msgId)
{
   return (msgId >= MSG_ENV_ID) && (msgId < MSG_ID_INVALID);
}

// TODO: Function which initializes a message + it's payload buffer to a known size

int
bc_msg_init(const uint32_t stationId, const uint16_t msgId, size_t payloadSize, bc_msg_t *const pMsg)
{
   if (pMsg == NULL)
   {
      // Failure by null pointer
      return -1;
   }

   if (!bc_is_msg_id_ok(msgId))
   {
      // Failure by invalid message ID
      return -1;
   }

   pMsg->station_id = stationId;
   pMsg->msg_id = msgId;

   // Allocate payload - REQUIRES FREED
   pMsg->msg_paylod = malloc(payloadSize);
   if (pMsg->msg_paylod == NULL)
   {
      // Failure by no mem
      return -1;
   }

   // On success, assume size is the payload size
   pMsg->payload_len = payloadSize;

   return 0;
}

int
bc_msg_parse(bc_msg_t *const pMsg, uint8_t *const buf, size_t buflen)
{

   uint64_t stationId = 0;
   uint16_t msgId = 0;

   if (pMsg == NULL || buf == NULL)
   {
      // Failure by null pointer
      return -1;
   }

   msgId = ntohs(*((uint16_t *)(buf + 4)));

   if (!bc_is_msg_id_ok(msgId))
   {
      // Failure by invalid message ID
      return -1;
   }

   stationId = ntohl(*((uint64_t *)(buf + 0)));

   // Initialize the message w/ a payload buffer. Must be freed
   if (bc_msg_init(stationId, msgId, buflen - MSG_HEAD_LEN, pMsg) != 0)
   {
      // On failure, this does not allocate any data
      return -1;
   }

   // Add the payload to the message
   if (bc_msg_add_payload(pMsg, buf + MSG_HEAD_LEN, buflen - MSG_HEAD_LEN) != 0){
      return -1;
   }

   return 0;
}

int
bc_msg_get_buf(bc_msg_t *const pMsg, uint8_t *const buf, size_t bufLen)
{
   if (pMsg == NULL || buf == NULL)
   {
      // Fail by null pointer
      return -1;
   }

   if (pMsg->msg_paylod == NULL)
   {
      // Fail by no payload
      return -1;
   }

   if (pMsg->payload_len + MSG_HEAD_LEN < bufLen)
   {
      // Fail by not long enough buffer
      return -1;
   }

   *((uint32_t *)(buf + 0)) = htonl(pMsg->station_id);
   *((uint16_t *)(buf + 4)) = htons(pMsg->msg_id);
   memcpy((buf + 6), pMsg->msg_paylod, MSG_ENV_LEN);

   return 0;
}

int bc_msg_add_payload(bc_msg_t *const pMsg, uint8_t *const buf, size_t bufLen){

   if (pMsg == NULL || buf == NULL || pMsg->msg_paylod == NULL){
      // Failure by null pointer
      return -1;
   }

   if (bufLen == 0 || bufLen != pMsg->payload_len){
      // Failure by invalid size
      return -1;
   }

   if (memcpy(pMsg->msg_paylod, buf, bufLen) == NULL){
      return -1;
   }

   return 0;
}
