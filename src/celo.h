/**
 * @file celo.h
 * @brief Header file containing function declarations and definitions for the Celo application.
 */

#pragma once

#include <stdint.h>
#include "ethUstream.h"
#include "types.h"

/**
 * @brief Formats the signature output.
 *
 * @param signature The signature to be formatted.
 * @param out The output buffer where the formatted signature will be stored.
 */
void format_signature_out(const uint8_t *signature, uint8_t *out);

/**
 * @brief Resets the application context.
 */
void reset_app_context();

/**
 * @brief Retrieves the known token based on the token address and chain ID.
 *
 * @param tokenAddr The address of the token.
 * @param chainId The chain ID for the token (0 for legacy compatibility).
 * @return A pointer to the token definition.
 */
tokenDefinition_t *getKnownToken(uint8_t *tokenAddr, uint64_t chainId);

/**
 * @brief Retrieves the index of a token based on its address and chain ID.
 *
 * @param addr The address of the token.
 * @param chainId The chain ID for the token (0 for legacy compatibility).
 * @return The index of the token. -1 if not found.
 */
int get_token_index_by_addr(const uint8_t *addr, uint64_t chainId);

/**
 * @brief Legacy function to get token by address only (for backward compatibility).
 *
 * @param tokenAddr The address of the token.
 * @return A pointer to the token definition.
 */
tokenDefinition_t *getKnownTokenLegacy(uint8_t *tokenAddr);

/**
 * @brief Gets the current transaction chain ID from the transaction content.
 *
 * @return The chain ID as uint64_t, or 0 if not available.
 */
uint64_t getCurrentTransactionChainId(void);

/**
 * @brief Custom processor for transaction context.
 *
 * @param context The transaction context.
 * @return The custom status code.
 */
customStatus_e customProcessor(txContext_t *context);

/**
 * @brief Initializes the transaction context.
 *
 * @param context The transaction context.
 * @param sha3 The SHA3 context.
 * @param content The transaction content.
 * @param customProcessor The custom processor function.
 * @param store_calldata Flag indicating if the calldata should be stored.
 * @param extra Additional data for the custom processor.
 */
void initTx(txContext_t *context,
            cx_sha3_t *sha3,
            txContent_t *content,
            ustreamProcess_t customProcessor,
            bool store_calldata,
            void *extra);

/**
 * @brief Finalizes the parsing process.
 *
 * @param direct Flag indicating if the parsing is direct.
 * @param use_standard_ui Flag indicating if the standard UI should be used.
 */
void finalizeParsing(bool direct, bool use_standard_ui);

/**
 * @brief Displays the transaction on the user interface.
 */
void ui_display_transaction(void);

extern volatile uint8_t appState; /**< The application state. */
