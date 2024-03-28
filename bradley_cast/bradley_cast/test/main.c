#include <stdio.h>
#include <string.h>
#include <vcruntime.h>
#include "bradley_cast_defs.h"
#include "env_msg.h"

#define FAKE_UUID     (0x0000000000000001)

#define TEST_LOG(MSG) (printf("[%s] " MSG "\n", __func__))

typedef int (*test_fn)();

// Test functions. Return 0 on success, non-zero value on failure
static void
print_hex_buffer(const uint8_t *const buf, const size_t bufLen)
{
   for (size_t i = 0; i < bufLen; ++i)
   {
      printf("%02X ", buf[i]);
   }
   printf("\n");
}

static int
test_env_message_construct()
{
   // DATA TO ENCODE
   // 22.5 degrees celsius
   // 100.5 kpa pressure
   // 45 percent humidity
   // is rainy
   // is currently sunny
   bc_env_data_t data = {0};
   bc_msg_t msg = {0};
   uint8_t expected_payload[MSG_ENV_LEN] = {
       0x08,
       0xCA,
       0x11,
       0x94,
       0x27,
       0x42,
       0x05,
   };

   data.temp_c = 22.5f;
   data.humid_prcnt = 45.0f;
   data.press_kpa = 100.5f;
   data.is_rainy = 1;
   data.light_level = BC_GLOOMY;

   // Try to build message
   TEST_LOG("Building environment message...");
   if (bc_build_env_msg(FAKE_UUID, &data, &msg) != 0)
   {
      TEST_LOG("Failed to build environment message");
      return -1;
   }
   else
   {
      TEST_LOG("Successfully built environment message");
   }

   // Check payload
   TEST_LOG("Checking payload contents...");
   if (memcmp(&expected_payload, msg.msg_paylod, MSG_ENV_LEN) == 0)
   {
      TEST_LOG("Payload contents good.");
   }
   else
   {
      TEST_LOG("Payload contents not as expected.");
      printf("\tEXPECTED PAYLOAD... ");
      print_hex_buffer(expected_payload, MSG_ENV_LEN);
      printf("\tPARSED PAYLOAD..... ");
      print_hex_buffer(msg.msg_paylod, MSG_ENV_LEN);
      return -1;
   }

   return 0;
}

static void
do_test(const char *const name, test_fn testFunction)
{
   printf("PERFORMING TEST: [%s]\n", name);
   if (testFunction() != 0)
   {
      printf("TEST: %s FAILED\n", name);
   }
   else
   {
      printf("TEST: %s PASSED\n", name);
   }
}

int
main(int argc, char *argv[])
{
   do_test("CONSTRUCT ENV MESSAGE", test_env_message_construct);
   printf("\n");
}
