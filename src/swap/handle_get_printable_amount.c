#ifdef HAVE_SWAP
#include <string.h>  // memset, explicit_bzero
#include "swap.h"
#include "globals.h"
#include "swap_utils.h"
#include "ethUtils.h"
#include "utils.h"


/* Set empty printable_amount on error, printable amount otherwise */
void swap_handle_get_printable_amount(get_printable_amount_parameters_t* params) {    
    uint8_t decimals;
    char ticker[MAX_TICKER_LEN] = {0};

    if (params->is_fee || params->coin_configuration == NULL) {
        memcpy(ticker, "CELO", sizeof("CELO"));
        decimals = CELO_PRECISION;
    } else {
        if (!swap_parse_config(params->coin_configuration,
                               params->coin_configuration_length,
                               ticker,
                               sizeof(ticker),
                               &decimals)) {
            PRINTF("Fail to parse coin_configuration\n");
            goto error;
        }
    }

    explicit_bzero(params->printable_amount, sizeof(params->printable_amount));
    if (params->amount_length > 32) {
        PRINTF("Amount is too big, 32 bytes max but buffer has %u bytes", params->amount_length);
        goto error;
    }
    if (!amountToString(params->amount,
                        params->amount_length,
                        decimals,
                        ticker,
                        params->printable_amount,
                        sizeof(params->printable_amount))) {
        PRINTF("Fail to prepare printable amount\n");
        goto error;
    }

    PRINTF("Amount %s\n", params->printable_amount);
    return; 
error:
    explicit_bzero(params->printable_amount, sizeof(params->printable_amount));
}

#endif // HAVE_SWAP