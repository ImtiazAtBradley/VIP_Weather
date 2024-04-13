#include <stdio.h>
#include <string.h>
#include <vcruntime.h>
#include "bradley_cast.h"
#include "bradley_cast_defs.h"
#include "env_msg.h"

#define FAKE_UUID     (0x00000001)

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
   // is currently gloomy
   bc_env_data_t data = {0};
   bc_msg_t msg = {0};
   uint8_t msg_buffer[MSG_ENV_LEN + MSG_HEAD_LEN] = {0};
   uint8_t expected_payload[MSG_ENV_LEN] = {
       0x08,
       0xCA,
       0x11,
       0x94,
       0x27,
       0x42,
       0x05,
   };
   uint8_t expected_msg[MSG_ENV_LEN + MSG_HEAD_LEN] = {
       0x00,
       0x00,
       0x00,
       0x01,
       0x00,
       0x10,
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
      TEST_LOG("FAILED to build environment message");
      bc_free_msg_payload(&msg);
      return -1;
   }
   else
   {
      TEST_LOG("SUCCESSFULLY built environment message");
   }

   // Check payload
   TEST_LOG("Checking payload contents...");
   if (memcmp(&expected_payload, msg.msg_paylod, MSG_ENV_LEN) == 0)
   {
      TEST_LOG("Payload contents GOOD.");
   }
   else
   {
      TEST_LOG("Payload contents NOT as expected.");
      printf("\tEXPECTED PAYLOAD... ");
      print_hex_buffer(expected_payload, MSG_ENV_LEN);
      printf("\tPARSED PAYLOAD..... ");
      print_hex_buffer(msg.msg_paylod, MSG_ENV_LEN);
      bc_free_msg_payload(&msg);
      return -1;
   }

   TEST_LOG("Building full message buffer...");
   if (bc_msg_get_buf(&msg, msg_buffer, MSG_ENV_LEN + MSG_HEAD_LEN) == 0)
   {
      TEST_LOG("SUCCESSFULLY constructed message buffer");
   }
   else
   {
      TEST_LOG("FAILED to construct message buffer");
      bc_free_msg_payload(&msg);
      return -1;
   }

   TEST_LOG("Checking full message buffer...");
   if (memcmp(msg_buffer, expected_msg, MSG_ENV_LEN + MSG_HEAD_LEN) == 0)
   {
      TEST_LOG("Full message buffer is OK");
   }
   else
   {
      TEST_LOG("Full message buffer is NOT as expected");
      bc_free_msg_payload(&msg);
      return -1;
   }

   return 0;
}

int
test_env_msg_parse()
{
   // DATA IN MSG
   // 22.5 degrees celsius
   // 100.5 kpa pressure
   // 45 percent humidity
   // is rainy
   // is currently sunny
   uint8_t sourceBuffer[MSG_ENV_LEN + MSG_HEAD_LEN] = {
       0x00,
       0x00,
       0x00,
       0x01,
       0x00,
       0x10,
       0x08,
       0xCA,
       0x11,
       0x94,
       0x27,
       0x42,
       0x05,
   };

   bc_msg_t msg = {0};
   bc_env_data_t envData = {0};

   TEST_LOG("Attempting to parse message from buffer...");
   if (bc_msg_parse(&msg, sourceBuffer, MSG_ENV_LEN + MSG_HEAD_LEN) != 0)
   {
      // Does not allocate anything on failure
      TEST_LOG("FAILED to parse message from buffer");
      return -1;
   }
   else
   {
      TEST_LOG("SUCESSFULLY parsed message from buffer");
   }

   TEST_LOG("Checking parsed data...");
   if (msg.msg_id == MSG_ENV_ID && msg.station_id == FAKE_UUID && msg.payload_len == MSG_ENV_LEN)
   {
      TEST_LOG("Parsed data inside of message structure GOOD.");
   }
   else
   {
      TEST_LOG("Parsed data inside of message structure BAD.");
      bc_free_msg_payload(&msg);
      return -1;
   }

   TEST_LOG("Parsing environment message data...");
   if (bc_parse_env_msg(&envData, &msg) == 0)
   {
      TEST_LOG("SUCCESFULLY parsed environment data from payload");
   }
   else
   {
      TEST_LOG("FAILED to parse environment data from payload");
      bc_free_msg_payload(&msg);
      return -1;
   }

   TEST_LOG("Checking parsed enviornmnet data...");
   if (envData.temp_c == 22.5f && envData.humid_prcnt == 45.0f && envData.press_kpa == 100.5f && envData.is_rainy
       && envData.light_level == BC_GLOOMY)
   {
      TEST_LOG("Parsed environment data is GOOD");
   } else {
      TEST_LOG("Parsed environment data is INCORRECT");
      printf(
         "Current data:\n"
         "\ttemp_c.........%f\n"
         "\thumid_prcnt....%f\n"
         "\tpress_kpa......%f\n"
         "\tis_rainy.......%u\n"
         "\tlight_level....%d\n",
         envData.temp_c,
         envData.humid_prcnt,
         envData.press_kpa,
         envData.is_rainy,
         envData.light_level
      );
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
   printf("\n");
}

int
main(int argc, char *argv[])
{
   do_test("CONSTRUCT ENV MESSAGE", test_env_message_construct);
   do_test("PARSE ENV MESSAGE", test_env_msg_parse);
   printf("\n");
}
