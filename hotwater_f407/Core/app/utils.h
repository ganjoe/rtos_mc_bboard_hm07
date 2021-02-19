/*
 * utils.h
 *
 *  Created on: 14.07.2020
 *      Author: daniel
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_
#include "stdint.h"
#include "stdbool.h"
#define TOBCD(x)        (((x) / 10 * 16) + ((x) % 10))
#define FROMBCD(x)      (((x) >> 4) * 10 + ((x) & 0xf))

int utils_truncate_number(float *number, float min, float max);
int utils_truncate_int_ptr(int *number, int *min, int *max);
int utils_truncate_number_int(int *number, int min, int max);

int utils_truncate_number_long(int64_t *number, int64_t min, int64_t max);

int utils_truncate_number_abs(float *number, float max);

float utils_map(float x, float in_min, float in_max, float out_min,
	float out_max);

void utils_map_ptr(float *x, float in_min, float in_max, float out_min,
	float out_max);

int utils_map_int(int x, int in_min, int in_max, int out_min, int out_max);

void utils_deadband(float *value, float tres, float max);

void utils_swap_bytes_in_word(uint16_t* word);
uint16_t utils_swap_bytes(uint16_t word, uint8_t* lbyte, uint8_t* hbyte);

void utils_set_bit_in_Word(uint16_t *word, uint8_t pos, bool state);
void utils_get_bytes_in_word(uint16_t* word, uint8_t* lbyte, uint8_t* hbyte);
void utils_get_word_from_bytes_ptr(uint16_t* word, uint8_t* lbyte, uint8_t* hbyte, int swap);
uint16_t utils_get_word_from_bytes(uint8_t lbyte, uint8_t hbyte, int swap);
void utils_togg_bit_in_Word(uint16_t *word, uint8_t pos);

int utils_get_bit_in_Word(uint16_t *word, uint16_t  pos);
int utils_get_Bit_in_Byte(uint8_t *byte, uint8_t  pos);
int utils_get_bit_in_dWord(uint32_t *word, uint32_t  pos);
int utils_get_bits_in_dWord(uint32_t *word, uint32_t bitmask);
int utils_get_bits_in_Word(uint16_t *word, uint16_t bitmask);
void utils_set_bits_in_dWord(uint32_t *word, uint32_t bitmask, int state);
void utils_set_bits_in_Word(uint16_t *word, uint16_t bitmask, int state);
int utils_get_inv_bit_in_Word(uint32_t *word, uint32_t pos);
int utils_get_inv_bits_in_dWord(uint32_t *word, uint32_t bitmask);

#endif /* INC_UTILS_H_ */
