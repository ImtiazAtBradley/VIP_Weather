#ifndef __BRADLEY_CAST_H__
#define __BRADLEY_CAST_H__

#include "bradley_cast_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

   int bc_free_msg_payload(bc_msg_t *const pMsg);

   int bc_is_msg_id_ok(const bc_msg_id_t msgId);

// INCLUDE MESSAGES FOR USERS TO CONSUME
#include "env_msg.h"

#ifdef __cplusplus
}
#endif

#endif // !__BRADLEY_CAST_H__
