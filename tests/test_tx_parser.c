#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../src_common/ethUstream.h"

static void test_celo_tx(void **state) {
  (void) state;

  const uint8_t tx_data[] = {
    0xEE, 0x7B, 0x82, 0x05, 0x39, 0x82, 0x52, 0x08, 0x80, 0x80, 0x80, 0x94,
    0xE7, 0x0E, 0x8A, 0xFE, 0xF8, 0x7C, 0xC8, 0xF0, 0xD7, 0xA6, 0x1F, 0x58,
    0x53, 0x5F, 0x6E, 0xC9, 0x9C, 0xD8, 0x60, 0xCA, 0x8A, 0x02, 0x8A, 0x85,
    0x74, 0x25, 0x46, 0x6F, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80
  };

  txContext_t context;
  txContent_t content;
  cx_sha3_t sha3;

  initTx(&context, &sha3, &content, NULL, NULL);
  assert_int_equal(processTx(&context, tx_data, sizeof(tx_data), 0), USTREAM_FINISHED);
}

int main(void) {
    const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_celo_tx),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
