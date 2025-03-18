/*
 * aes.h
 *
 *  Created on: Mar 17, 2025
 *      Author: thoma
 */

#ifndef AES_H
#define AES_H

#include <stdint.h>

#define AES_BLOCK_SIZE 16  // Taille d'un bloc AES (128 bits)
#define AES_KEY_SIZE 16     // Taille de la clé AES-128 (128 bits)
#define AES_ROUNDS 10       // Nombre de rounds pour AES-128
#define WORD_LENGTH 4

#define AES_128_ROUND_NUMBER 11
#define AES_192_ROUND_NUMBER 13
#define AES_256_ROUND_NUMBER 15


void aes_key_expansion(const uint8_t *key, uint8_t *roundKeys);
void aes_encrypt_block(const uint8_t *input, uint8_t *output, const uint8_t *roundKeys);

#endif // AES_H
