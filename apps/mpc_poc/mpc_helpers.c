#include "mpc_helpers.h"

#include "ecdsa.h"
#include "curves.h"
#include "bip32.h"
#include "coin_specific_data.h"

#include <stdio.h>

void mpc_sign_message(const uint8_t *message, size_t message_len, uint8_t *sig, const uint8_t *priv_key) {
    uint8_t pby;
    const ecdsa_curve *curve = get_curve_by_name(SECP256K1_NAME)->params;
    ecdsa_sign(curve, HASHER_SHA2D, priv_key, message, message_len, sig, &pby, NULL);
}

bool mpc_verify_signature(const uint8_t *message, size_t message_len, const uint8_t *sig, const uint8_t *pub_key) {
    const ecdsa_curve *curve = get_curve_by_name(SECP256K1_NAME)->params;
    return (ecdsa_verify(curve, HASHER_SHA2D, pub_key, sig, message, message_len) == 0);
}

void bytes_to_hex(const uint8_t *data, size_t data_len, char *out, size_t out_len) {
    char *ptr = out;
    size_t remaining = out_len;
    size_t num_written;

    for (size_t i = 0; i < data_len; ++i) {
        // Each byte will produce two hexadecimal characters
        num_written = snprintf(ptr, remaining, "%02X", data[i]);

        if (num_written >= remaining) {
            // Not enough space in the buffer, stop writing
            break;
        }

        ptr += num_written; // Move the pointer forward
        remaining -= num_written; // Decrease the remaining space
    }

    // Ensure the output is null-terminated
    if (out_len > 0) {
        out[out_len - 1] = '\0';
    }
}

int fetch_pub_key_from_flash(const uint8_t *wallet_id, uint8_t *pub_key) {
    uint8_t priv_key[32] = {0};

    if (fetch_priv_key_from_flash(wallet_id, priv_key) != 0) {
        memzero(priv_key, sizeof(priv_key));
        return 1;
    }

    pub_key33_from_priv_key(priv_key, pub_key);
    memzero(priv_key, sizeof(priv_key));
    
    return 0;
}

int fetch_priv_key_from_flash(const uint8_t *wallet_id, uint8_t *priv_key) {
    const uint16_t PRIV_KEY_LEN = 32;
    uint16_t len_read = 0;
    Coin_Specific_Data_Struct coin_specific_data;

    coin_specific_data.coin_type = MPC_APP;
    memcpy(coin_specific_data.wallet_id, wallet_id, WALLET_ID_SIZE);
    coin_specific_data.coin_data = priv_key;

    if (get_coin_data(&coin_specific_data, sizeof(priv_key), &len_read) != 0 || 
        len_read != PRIV_KEY_LEN) {

        memzero(priv_key, sizeof(priv_key));
        return 1;
    }

    return 0;
}

void priv_key_from_seed(const uint8_t *seed, uint8_t *priv_key) {
    const size_t SEED_LEN = 64;
    uint8_t sha256_hash[32];

    Hasher hasher;
    hasher_Init(&hasher, HASHER_SHA2);  

    hasher_Update(&hasher, seed, SEED_LEN);
    hasher_Final(&hasher, sha256_hash);

    for (int i = 0; i < 32; i++) {
        priv_key[i] = sha256_hash[i];
    }
}

void pub_key33_from_priv_key(const uint8_t *priv_key, uint8_t *pub_key) {
    const ecdsa_curve *curve = get_curve_by_name(SECP256K1_NAME)->params;
    ecdsa_get_public_key33(curve, priv_key, pub_key);
}