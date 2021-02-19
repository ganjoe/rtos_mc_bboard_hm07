#include "../utils.h"
#include "math.h"



int utils_truncate_number(float *number, float min, float max)
    {
    int did_trunc = 0;

    if (*number > max)
	{
	*number = max;
	did_trunc = 1;
	}
    else if (*number < min)
	{
	*number = min;
	did_trunc = 1;
	}

    return did_trunc;
    }
int utils_truncate_int_ptr(int *number, int *min, int *max)
    {
    int did_trunc = 0;

    if (*number > max)
	{
	*number = max;
	did_trunc = 1;
	}
    else if (*number < min)
	{
	*number = min;
	did_trunc = 1;
	}

    return did_trunc;
    }
//umbenennen
int utils_truncate_number_int(int *number, int min, int max)
    {
    int did_trunc = 0;

    if (*number > max)
	{
	*number = max;
	did_trunc = 1;
	}
    else if (*number < min)
	{
	*number = min;
	did_trunc = 1;
	}

    return did_trunc;
    }


int utils_truncate_number_abs(float *number, float max)
    {
    int did_trunc = 0;

    if (*number > max)
	{
	*number = max;
	did_trunc = 1;
	}
    else if (*number < -max)
	{
	*number = -max;
	did_trunc = 1;
	}

    return did_trunc;
    }

float utils_map(float x, float in_min, float in_max, float out_min,	float out_max)
    {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

void utils_map_ptr(float *x, float in_min, float in_max, float out_min,
	float out_max)
    {
    *x = (*x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

int utils_map_int(int x, int in_min, int in_max, int out_min, int out_max)
    {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

/**
 * Truncate absolute values less than tres to zero. The value
 * tres will be mapped to 0 and the value max to max.
 */
void utils_deadband(float *value, float tres, float max)
    {
    if (fabsf(*value) < tres)
	{
	*value = 0.0;
	}
    else
	{
	float k = max / (max - tres);
	if (*value > 0.0)
	    {
	    *value = k * *value + max * (1.0 - k);
	    }
	else
	    {
	    *value = -(k * -*value + max * (1.0 - k));
	    }
	}
    }

void utils_swap_bytes_in_word(uint16_t* word)
    {
    uint16_t dword=*word;
    uint8_t lowbyte, highbyte;
    lowbyte = dword >> 8;
    highbyte = dword;
    dword = highbyte << 8;
    dword |= lowbyte;
    *word = dword;
    }

uint16_t utils_swap_bytes(uint16_t word, uint8_t* lbyte, uint8_t* hbyte)
    {
    uint16_t dword=word;
    uint8_t lowbyte, highbyte;
    lowbyte = dword >> 8;
    highbyte = dword;
    dword = highbyte << 8;
    dword |= lowbyte;
    *lbyte = lowbyte;
    *hbyte = highbyte;
    return dword;
    }

void utils_get_bytes_in_word(uint16_t* word, uint8_t* lbyte, uint8_t* hbyte)
    {
    uint16_t dword=*word;
    uint8_t lowbyte=*lbyte;
    uint8_t highbyte=*hbyte;
    highbyte = dword >> 8;
    lowbyte = dword;
    *lbyte = lowbyte;
    *hbyte =highbyte;

    }

uint16_t utils_get_word_from_bytes(uint8_t lbyte, uint8_t hbyte, int swap)
    {
    uint8_t dword[2];
    uint16_t word;
    dword[0] =lbyte;
    dword[1] =hbyte;
    word = dword[!swap];
    word *= 0x100;
    word |= dword[swap];
    return word;
    }

void utils_get_word_from_bytes_ptr(uint16_t* word, uint8_t* lbyte, uint8_t* hbyte, int swap)
    {

    }

void utils_set_bit_in_Word(uint16_t *word, uint8_t pos, bool state)
	{
	switch (state)
	    {
	    case 1:
		*word |= 1UL << pos;
		break;
	    case 0:
		*word &= ~(1UL << pos);
		break;
	    }

	}

void utils_togg_bit_in_Word(uint16_t *word, uint8_t pos)
    {
    *word ^=1<<pos;
    }

int utils_get_Bit_in_Byte(uint8_t *byte, uint8_t  pos)
{
    if (*byte & (1<<pos)) 	{return true;}
    else 					{return false;}
}

int utils_get_bit_in_Word(uint16_t *word, uint16_t  pos)
{
    if (*word & (1<<pos)) 	{return true;}
    else 					{return false;}
}
int utils_get_bit_in_dWord(uint32_t *word, uint32_t  pos)
{
    if (*word & (1<<pos)) 	{return true;}
    else 					{return false;}
}
int utils_get_inv_bit_in_Word(uint32_t *word, uint32_t pos)
{
	uint16_t lword;
	lword = *word;
	lword = ~lword;
    if ((lword) & (1<<pos)) 	{return true;}
    else 						{return false;}
}


int utils_get_bits_in_Word(uint16_t *word, uint16_t bitmask)
{
    if ((*word & bitmask) == bitmask)
    {
    	return true;
    }
    else
    {
    	return false;
    }

}

void utils_set_bits_in_dWord(uint32_t *word, uint32_t bitmask, int state)
{
	switch (state)
	{
	case true:
	*word |= bitmask;
	break;
	case false:
	*word &= ~(bitmask);
	break;
	}

}
void utils_set_bits_in_Word(uint16_t* word, uint16_t bitmask, int state)
{
	switch (state)
	{
	case true:
	*word |= bitmask;
	break;
	case false:
	*word &= ~(bitmask);
	break;
	}

}
int utils_get_inv_bits_in_dWord(uint32_t *word, uint32_t bitmask)
{
	uint32_t lword = 0;
	lword = ~(*word);

    if ((lword & bitmask) == bitmask)
    	{
    	return true;
    	}
    else
    	{
    	return false;
    	}
}
int utils_get_bits_in_dWord(uint32_t *word, uint32_t bitmask)
{
    if ((*word & bitmask) == bitmask)
    {
    	return true;
    }
    else
    {
    	return false;
    }

}
