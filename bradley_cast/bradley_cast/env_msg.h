#ifndef __ENV_MSG_H__
#define __ENV_MSG_H__

#include "bradley_cast_defs.h"

int bc_build_env_msg(const uint32_t stationId, bc_env_data_t *const pEnvData, bc_msg_t *const pMsg);

int bc_parse_env_msg(bc_env_data_t *const pEnvData, bc_msg_t *const pMsg);

#endif // __ENV_MSG_H__
