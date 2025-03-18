#include "../../Inc/aes/aes.h"
#include <string.h>

// S-Box utilisée dans l'étape SubBytes
static const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};


// Fonction AddRoundKey : applique XOR entre state et la clé de tour
void add_round_key(uint8_t *state, const uint8_t *roundKey) {
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        state[i] ^= roundKey[i];  // XOR chaque octet du bloc avec la clé de tour
    }
}

// Fonction pour afficher le contenu du bloc AES
void print_state(const uint8_t *state) {
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%02X ", state[i]);  // Affiche chaque octet en hexadécimal
    }
    printf("\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

// Fonction SubBytes : remplace chaque octet par sa valeur dans la S-Box
void sub_bytes(uint8_t *state) {
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        state[i] = sbox[state[i]];
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

// Fonction ShiftRows : décale les lignes de la matrice AES
void shift_rows(uint8_t *state) {
    uint8_t temp;

    // Décalage de la ligne 1 (1 octet à gauche)
    temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = temp;

    // Décalage de la ligne 2 (2 octets à gauche)
    temp = state[2];
    state[2] = state[10];
    state[10] = temp;

    temp = state[6];
    state[6] = state[14];
    state[14] = temp;

    // Décalage de la ligne 3 (3 octets à gauche)
    temp = state[3];
    state[3] = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = temp;
}

//----------------------------------------------------------------------------------------------------------------------------------
typedef uint8_t AES_BYTE;

/* Multiplication dans GF(2^8) avec le polynôme irréductible AES (x^8 + x^4 + x^3 + x + 1) */
AES_BYTE gmul(AES_BYTE a, AES_BYTE b) {
    AES_BYTE p = 0;
    while (a && b) {
        if (b & 1) p ^= a;
        if (a & 0x80) {
            a = (a << 1) ^ 0x1b;
        } else {
            a <<= 1;
        }
        b >>= 1;
    }
    return p;
}

void mixColumn(AES_BYTE *col) {
    AES_BYTE t[4];
    t[0] = gmul(col[0], 0x02) ^ gmul(col[1], 0x03) ^ col[2] ^ col[3];
    t[1] = col[0] ^ gmul(col[1], 0x02) ^ gmul(col[2], 0x03) ^ col[3];
    t[2] = col[0] ^ col[1] ^ gmul(col[2], 0x02) ^ gmul(col[3], 0x03);
    t[3] = gmul(col[0], 0x03) ^ col[1] ^ col[2] ^ gmul(col[3], 0x02);

    for (int i = 0; i < 4; i++) {
        col[i] = t[i];
    }
}

void mixColumns(AES_BYTE *state) {
    for (int i = 0; i < 4; i++) {
        mixColumn(state + i * 4);
    }
}
//-----------------------------------------------------------------------------------------------------------------------



uint8_t sbox_substitution(uint8_t byte){
	return sbox[byte];
}


uint8_t XOR_bytes(const uint8_t byte1, const uint8_t byte2){
	return byte1^byte2;
}


void XOR_words(const uint8_t * word1, const uint8_t * word2, uint8_t * ret){

	for(int i = 0; i < WORD_LENGTH; ++i){
		ret[i] = XOR_bytes(word1[i],word2[i]);
	}
}

uint8_t get_round_constant(const int n){
	const uint8_t round_constants[AES_128_ROUND_NUMBER] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
	return round_constants[n];
}


void g_function(const uint8_t * word, const int round, uint8_t * ret){

	//shift left
	ret[0] = word[1];
	ret[1] = word[2];
	ret[2] = word[3];
	ret[3] = word[0];

	//sbox substitution
	ret[0] = sbox_substitution(ret[0]);
	ret[1] = sbox_substitution(ret[1]);
	ret[2] = sbox_substitution(ret[2]);
	ret[3] = sbox_substitution(ret[3]);

	//round constant XOR
	const uint8_t round_constant = get_round_constant(round);
	const uint8_t round_constant_word[WORD_LENGTH] = { round_constant, 0x00, 0x00, 0x00};

	XOR_words(ret,round_constant_word, ret);

}



void key_expansion_by_round(const uint8_t * key , const int round, uint8_t * ret_key){


	//select the 4 words from the key
	const uint8_t word0[WORD_LENGTH] = {key[0],  key[1],  key[2],  key[3]};
	const uint8_t word1[WORD_LENGTH] = {key[4],  key[5],  key[6],  key[7]};
	const uint8_t word2[WORD_LENGTH] = {key[8],  key[9],  key[10], key[11]};
	const uint8_t word3[WORD_LENGTH] = {key[12], key[13], key[14], key[15]};

	//apply g function and XOR
	uint8_t word4[WORD_LENGTH];
	uint8_t word5[WORD_LENGTH];
	uint8_t word6[WORD_LENGTH];
	uint8_t word7[WORD_LENGTH];

	uint8_t g_function_word[WORD_LENGTH];
	g_function(word3,round, g_function_word);

	XOR_words(g_function_word , word0, word4);
	XOR_words(word4, word1, word5);
	XOR_words(word5, word2, word6);
	XOR_words(word6, word3, word7);

	//build key
	uint8_t temp_key[16 * WORD_LENGTH] = {word4[0], word4[1], word4[2], word4[3],
										  word5[0], word5[1], word5[2], word5[3],
										  word6[0], word6[1], word6[2], word6[3],
										  word7[0], word7[1], word7[2], word7[3]};

	//change return key
	memcpy(ret_key, temp_key, AES_BLOCK_SIZE);
}

void AES_key_schedule(const uint8_t * key, uint8_t full_key_expansion[AES_128_ROUND_NUMBER][AES_BLOCK_SIZE * 16]){
	const int rounds = AES_128_ROUND_NUMBER - 1;

	memcpy(full_key_expansion[0], key, AES_BLOCK_SIZE); //round 0 key is initial key

	//build other keys
	for(int i = 0; i < rounds; ++i){
		const int n = i + 1;
		key_expansion_by_round(full_key_expansion[i], n, full_key_expansion[n]);
	}
}

void print_key(uint8_t * key){
	for(int i = 0; i < AES_BLOCK_SIZE; ++i){
		printf("%02X ", key[i]);
	}
	printf("\n");
}

//------------------------------------------------------------------------------------------------------------------------------

void aes_encrypt(uint8_t *state, uint8_t *key) {
	uint8_t full_key_expansion[AES_128_ROUND_NUMBER][AES_BLOCK_SIZE * 16];//uniquement pour AES-128

	AES_key_schedule(key, full_key_expansion);
	for(int i = 0; i < AES_128_ROUND_NUMBER; ++i){

		printf("Round %0X : ",i);
		print_key(full_key_expansion[i]);
	}

    printf("\n--- Début de l'encryption AES ---\n");
    printf("État initial:\n");
    print_state(state);

    add_round_key(state, full_key_expansion[0]);
    printf("Après AddRoundKey (Round 0):\n");
    print_state(state);

    for (int round = 1; round < AES_128_ROUND_NUMBER; round++) {
        printf("\n--- Début du Round %d ---\n", round);
        sub_bytes(state);
        printf("Après SubBytes:\n");
        print_state(state);

        shift_rows(state);
        printf("Après ShiftRows:\n");
        print_state(state);

        if (round < AES_128_ROUND_NUMBER - 1) {
            mixColumns(state);
            printf("Après MixColumns:\n");
            print_state(state);
        }
        printf("Round key used:\n");
        print_state(full_key_expansion[round]);

        add_round_key(state, full_key_expansion[round]);
        printf("Après AddRoundKey (Round %d):\n", round);
        print_state(state);
    }

    printf("\n--- Fin de l'encryption AES ---\n");
}
