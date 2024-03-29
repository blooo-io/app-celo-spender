#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <stdbool.h>

#include "../src_common/ethUstream.h"

static void test_process_tx(uint8_t txType, const uint8_t* tx_data, size_t tx_data_size, const uint8_t* expected_to, int expected_status) {
  txContext_t context;
  txContent_t content;
  cx_sha3_t sha3;

  initTx(&context, &sha3, &content, NULL, NULL);
  context.txType = txType;
  assert_int_equal(processTx(&context, tx_data, tx_data_size), expected_status);
  // do not perform other checks if expected_to is NULL
  if(expected_to == NULL) {
    return;
  }
  assert_int_equal(content.destinationLength, MAX_ADDRESS);
  assert_memory_equal(content.destination, expected_to, MAX_ADDRESS);
}

// static void test_celo_tx_invalid_address(void **state) {
//   (void) state;
//
//   // dest address set to 0xe70e8afef87cc8f0d7a61f58535f6ec99cd860 (19 bytes)
//   const uint8_t tx_data[] = {
//     0xEE, 0x7B, 0x82, 0x05, 0x39, 0x82, 0x52, 0x08, 0x80, 0x80, 0x80, 0x93,
//     0xE7, 0x0E, 0x8A, 0xFE, 0xF8, 0x7C, 0xC8, 0xF0, 0xD7, 0xA6, 0x1F, 0x58,
//     0x53, 0x5F, 0x6E, 0xC9, 0x9C, 0xD8, 0x60, 0x8A, 0x02, 0x8A, 0x85,
//     0x74, 0x25, 0x46, 0x6F, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80
//   };
//
//   test_process_tx(CELO_LEGACY, tx_data, sizeof(tx_data), NULL, USTREAM_FAULT);
// }

static void test_celo_legacy_tx(void **state) {
  (void) state;

  const uint8_t tx_data[] = {
    0xEE, 0x7B, 0x82, 0x05, 0x39, 0x82, 0x52, 0x08, 0x80, 0x80, 0x80, 0x94,
    0xE7, 0x0E, 0x8A, 0xFE, 0xF8, 0x7C, 0xC8, 0xF0, 0xD7, 0xA6, 0x1F, 0x58,
    0x53, 0x5F, 0x6E, 0xC9, 0x9C, 0xD8, 0x60, 0xCA, 0x8A, 0x02, 0x8A, 0x85,
    0x74, 0x25, 0x46, 0x6F, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80
  };

  const uint8_t to[] = {
    0xE7, 0x0E, 0x8A, 0xFE, 0xF8, 0x7C, 0xC8, 0xF0, 0xD7, 0xA6, 0x1F, 0x58,
    0x53, 0x5F, 0x6E, 0xC9, 0x9C, 0xD8, 0x60, 0xCA
  };

  test_process_tx(CELO_LEGACY, tx_data, sizeof(tx_data), to, 0x6501);
}

static void test_celo_eip1559_tx(void **state) {
  (void) state;
  // TO DO: IMPLEMENT
  const uint8_t tx_data[] = {};

  const uint8_t to[] = {};

  test_process_tx(EIP1559, tx_data, sizeof(tx_data), to, USTREAM_FINISHED);
}

static void test_celo_cip64_tx(void **state) {
  (void) state;
  // TO DO: IMPLEMENT
  const uint8_t tx_data[] = {};

  const uint8_t to[] = {};

  test_process_tx(CIP64, tx_data, sizeof(tx_data), to, USTREAM_FINISHED);
}



int main(void) {
    const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_celo_legacy_tx),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
