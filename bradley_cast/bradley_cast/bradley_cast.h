#ifndef __BRADLEY_CAST_H__
#define __BRADLEY_CAST_H__

#include "bradley_cast_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

   int bc_free_msg_payload(bc_msg_t *const pMsg);

   int bc_is_msg_id_ok(const bc_msg_id_t msgId);

   int bc_msg_init(const uint32_t stationId, const uint16_t msgId, size_t payloadSize, bc_msg_t *const pMsg);

   int bc_msg_parse(bc_msg_t *const pMsg, uint8_t *const buf, size_t buflen);

   int bc_msg_get_buf(bc_msg_t *const pMsg, uint8_t *const buf, size_t bufLen);

   int bc_msg_add_payload(bc_msg_t *const pMsg, uint8_t *const buf, size_t bufLen);

#ifdef __cplusplus
}
#endif

#endif // !__BRADLEY_CAST_H__
