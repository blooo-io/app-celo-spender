#include "provide_erc20_token_information.h"

#include "io.h"
#include "sw.h"
#include "globals.h"
#include "constants.h"
#include "celo.h"
#include "utils.h"
#include "ethUtils.h"
#include "ui_common.h"

// cLabs token signature public key
static const uint8_t CLABS_SIGNATURE_PUBLIC_KEY[] = {
    // cLabs production key (created: May 30, 2024, 17:18:49 GMT+2)
    // akeyless URL:
    // https://ui.gateway.akeyless.celo-networks-dev.org/items?id=281699245&name=%2Fstatic-secrets%2Fdev-tooling-circle%2Fledger-key
    // akeyless path: /static-secrets/dev-tooling-circle
    // Running `openssl ec -in key.pem -text -noout`
    // should output the same hex pub key
    0x04,

    0x59, 0xd5, 0x59, 0xee, 0x21, 0xa7, 0x76, 0xfe, 0x43, 0xe4, 0xba, 0xac, 0x14, 0x18, 0x1c, 0x1e,
    0x5f, 0xb5, 0x1b, 0x5b, 0x22, 0xbc, 0x01, 0xdd, 0x46, 0xe0, 0x60, 0x0e, 0x8d, 0x0c, 0xd7, 0xc8,

    0x6d, 0x0c, 0xf2, 0x3e, 0xe0, 0xa3, 0x8d, 0xc8, 0x3d, 0xa6, 0x4b, 0xd0, 0x2a, 0x6d, 0x43, 0x2c,
    0x86, 0x10, 0x4a, 0x47, 0xaf, 0xef, 0x83, 0x83, 0xc2, 0x2b, 0xe3, 0xd4, 0xd1, 0xa5, 0x32, 0x2d};

// Ledger CAL token signature public key
static const uint8_t LEDGER_SIGNATURE_PUBLIC_KEY[] = {
#if defined(HAVE_CAL_TEST_KEY)
    0x04, 0x4c, 0xca, 0x8f, 0xad, 0x49, 0x6a, 0xa5, 0x04, 0x0a, 0x00, 0xa7, 0xeb, 0x2f,
    0x5c, 0xc3, 0xb8, 0x53, 0x76, 0xd8, 0x8b, 0xa1, 0x47, 0xa7, 0xd7, 0x05, 0x4a, 0x99,
    0xc6, 0x40, 0x56, 0x18, 0x87, 0xfe, 0x17, 0xa0, 0x96, 0xe3, 0x6c, 0x3b, 0x52, 0x3b,
    0x24, 0x4f, 0x3e, 0x2f, 0xf7, 0xf8, 0x40, 0xae, 0x26, 0xc4, 0xe7, 0x7a, 0xd3, 0xbc,
    0x73, 0x9a, 0xf5, 0xde, 0x6f, 0x2d, 0x77, 0xa7, 0xb6
#elif defined(HAVE_CAL_STAGING_KEY)
    // staging key 2019-01-11 03:07PM (erc20signer)
    0x04, 0x20, 0xda, 0x62, 0x00, 0x3c, 0x0c, 0xe0, 0x97, 0xe3, 0x36, 0x44, 0xa1, 0x0f,
    0xe4, 0xc3, 0x04, 0x54, 0x06, 0x9a, 0x44, 0x54, 0xf0, 0xfa, 0x9d, 0x4e, 0x84, 0xf4,
    0x50, 0x91, 0x42, 0x9b, 0x52, 0x20, 0xaf, 0x9e, 0x35, 0xc0, 0xb2, 0xd9, 0x28, 0x93,
    0x80, 0x13, 0x73, 0x07, 0xde, 0x4d, 0xd1, 0xd4, 0x18, 0x42, 0x8c, 0xf2, 0x1a, 0x93,
    0xb3, 0x35, 0x61, 0xbb, 0x09, 0xd8, 0x8f, 0xe5, 0x79
#else
    // production key 2019-01-11 03:07PM (erc20signer)
    0x04, 0x5e, 0x6c, 0x10, 0x20, 0xc1, 0x4d, 0xc4, 0x64, 0x42, 0xfe, 0x89, 0xf9, 0x7c,
    0x0b, 0x68, 0xcd, 0xb1, 0x59, 0x76, 0xdc, 0x24, 0xf2, 0x4c, 0x31, 0x6e, 0x7b, 0x30,
    0xfe, 0x4e, 0x8c, 0xc7, 0x6b, 0x14, 0x89, 0x15, 0x0c, 0x21, 0x51, 0x4e, 0xbf, 0x44,
    0x0f, 0xf5, 0xde, 0xa5, 0x39, 0x3d, 0x83, 0xde, 0x53, 0x58, 0xcd, 0x09, 0x8f, 0xce,
    0x8f, 0xd0, 0xf8, 0x1d, 0xaa, 0x94, 0x97, 0x91, 0x83
#endif
};

/**
 * Handle Provide ERC20 Token Information command
 *
 * @param p1 The first parameter
 * @param p2 The second parameter
 * @param workBuffer The work buffer
 * @param dataLength The length of the data buffer
 * @param flags The flags
 * @param tx The transaction buffer
 */
void handleProvideErc20TokenInformation(uint8_t p1,
                                        uint8_t p2,
                                        uint8_t *workBuffer,
                                        uint16_t dataLength,
                                        volatile unsigned int *flags,
                                        volatile unsigned int *tx) {
    UNUSED(p1);
    UNUSED(p2);
    UNUSED(flags);
    UNUSED(tx);
    uint32_t offset = 0;
    uint8_t tickerLength;
    uint8_t hash[32];
    cx_ecfp_public_key_t tokenKey;
    for (int i = 0; i < MAX_ASSETS; i++) {
        PRINTF("\n[%d].address: ", i);
        for (int j = 0; j < ADDRESS_LENGTH; j++) {
            PRINTF("%02x", tmpCtx.transactionContext.extraInfo[i].token.address[j]);
        }
    }
    PRINTF("\n");

    tokenDefinition_t *token =
        &tmpCtx.transactionContext.extraInfo[tmpCtx.transactionContext.currentAssetIndex].token;

    PRINTF("Provisioning currentAssetIndex %d\n", tmpCtx.transactionContext.currentAssetIndex);

    if (dataLength < 1) {
        THROW(SW_ERROR_IN_DATA);
    }
    tickerLength = workBuffer[offset++];
    dataLength--;
    // We need to make sure we can write the ticker, a space and a zero byte at the end
    if ((tickerLength + 2) >= sizeof(token->ticker)) {
        THROW(SW_ERROR_IN_DATA);
    }
    if (dataLength < tickerLength + 20 + 4 + 4) {
        THROW(SW_ERROR_IN_DATA);
    }
    cx_hash_sha256(workBuffer + offset, tickerLength + 20 + 4 + 4, hash, 32);
    memcpy(token->ticker, workBuffer + offset, tickerLength);
    token->ticker[tickerLength] = ' ';
    token->ticker[tickerLength + 1] = '\0';
    offset += tickerLength;
    dataLength -= tickerLength;
    memcpy(token->address, workBuffer + offset, 20);
    offset += 20;
    dataLength -= 20;
    token->decimals = U4BE(workBuffer, offset);
    offset += 4;
    dataLength -= 4;

    // Read and store chainId instead of skipping it
    uint64_t chain_id = U4BE(workBuffer, offset);
    token->chain_id = chain_id;
    PRINTF("DEBUG: Parsed chain ID: %u (0x%08X)\n", (uint32_t) chain_id, (uint32_t) chain_id);
    PRINTF(
        "DEBUG: Token stored - Address: %02X%02X...%02X%02X, Ticker: %s, Decimals: %u, Chain ID: "
        "%u\n",
        token->address[0],
        token->address[1],
        token->address[18],
        token->address[19],
        token->ticker,
        token->decimals,
        (uint32_t) token->chain_id);
    offset += 4;
    dataLength -= 4;
    // Try cLabs key first
    CX_THROW(cx_ecfp_init_public_key_no_throw(CX_CURVE_256K1,
                                              CLABS_SIGNATURE_PUBLIC_KEY,
                                              sizeof(CLABS_SIGNATURE_PUBLIC_KEY),
                                              &tokenKey));

    if (!cx_ecdsa_verify_no_throw(&tokenKey, hash, 32, workBuffer + offset, dataLength)) {
        // Try Ledger CAL key
        CX_THROW(cx_ecfp_init_public_key_no_throw(CX_CURVE_256K1,
                                                  LEDGER_SIGNATURE_PUBLIC_KEY,
                                                  sizeof(LEDGER_SIGNATURE_PUBLIC_KEY),
                                                  &tokenKey));

        if (!cx_ecdsa_verify_no_throw(&tokenKey, hash, 32, workBuffer + offset, dataLength)) {
            PRINTF("Invalid token signature\n");
            THROW(SW_ERROR_IN_DATA);
        }
    }

    tmpCtx.transactionContext.assetSet[tmpCtx.transactionContext.currentAssetIndex] = 1;
    tmpCtx.transactionContext.currentAssetIndex =
        (tmpCtx.transactionContext.currentAssetIndex + 1) % MAX_ASSETS;
    THROW(SW_OK);
}
