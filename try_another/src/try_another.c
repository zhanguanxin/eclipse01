/*
 ============================================================================
 Name        : try_another.c
 Author      : guanxin
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>

typedef int bool;
#define false 0;
#define true 1;

unsigned char crc8(unsigned char *data, int length) //function of calculate the Key
{
unsigned char t_crc;
int f, b;
t_crc = 0xFF;
for (f = 0; f < length; f++)
{
t_crc ^= data[f];
for (b = 0; b < 8; b++)
{
if ((t_crc & 0x80) != 0)
{
t_crc <<= 1;
t_crc ^= 0x1D;
}
else
{
t_crc <<= 1;
}
}
}
return ~t_crc;
}

bool __declspec(dllexport) __cdecl ASAP1A_CCP_ComputeKeyFromSeed(char *seed, char * key, unsigned short *sizeKey)
{
int seedlength = 6;
unsigned char buf_byte[6];
unsigned char crc_byte[7];
//seed[4] = 0x12;//high byte of secret code(example)
//seed[5] = 0x34;//low byte of secret code(example)
buf_byte[0] = seed[0];
buf_byte[1] = seed[1];
buf_byte[2] = seed[2];
buf_byte[3] = seed[3];
buf_byte[4] = seed[4];
buf_byte[5] = seed[5];
crc_byte[0] = crc8(buf_byte,seedlength);
buf_byte[0] = crc_byte[0];
crc_byte[1] = crc8(buf_byte,seedlength);
buf_byte[0] = seed[0];
buf_byte[1] = crc_byte[1];
crc_byte[2] = crc8(buf_byte, seedlength);
buf_byte[1] = seed[1];
buf_byte[2] = crc_byte[2];
crc_byte[3] = crc8(buf_byte, seedlength);
buf_byte[2] = seed[2];
buf_byte[3] = crc_byte[3];
crc_byte[4] = crc8(buf_byte, seedlength);
buf_byte[3] = seed[3];
buf_byte[4] = crc_byte[4];
crc_byte[5] = crc8(buf_byte, seedlength);
buf_byte[4] = seed[4];
buf_byte[5] = crc_byte[5];
crc_byte[6] = crc8(buf_byte, seedlength);
if (crc_byte[3] == 0 && crc_byte[4] == 0 && crc_byte[5] == 0 && crc_byte[6] == 0)
{
key[0] = crc_byte[1];
key[1] = crc_byte[2];
key[2] = crc_byte[3];
key[3] = crc_byte[4];
}else
{
key[0] = crc_byte[3];
key[1] = crc_byte[4];
key[2] = crc_byte[5];
key[3] = crc_byte[6];
}
*sizeKey = 4;
// If the return value is false the flash tool stops
return true;
}



int main(void) {
	//char seedFromECU[6] = {0xF1,0xAF,0x9B,0x2E,121,-6};  //ADC level2
	char seedFromECU[6] = {0xFF,0xFF,0xFF,0xFF,0x12,0x34};
	//char seedFromECU[6] = {0xA3,0xB2,0x18,0x75,0x12,0x34};
	//char seedFromECU[6] = {0x22,0x85,0x55,0x70,-82,7};
	char keyToECU[4];
	unsigned short sizeKeyValue = 0;

	puts("!!!Hello World, XPEV0827!!!"); /* prints !!!Hello World!!! */
	ASAP1A_CCP_ComputeKeyFromSeed(seedFromECU, keyToECU, &sizeKeyValue);
	//printf("%hhX,%hhX\n", seedFromECU[4]&0xff,seedFromECU[5]&0xff);

	printf("%hhX,%hhX,%hhX,%hhX\n", keyToECU[0]&0xff, keyToECU[1]&0xff, keyToECU[2]&0xff, keyToECU[3]&0xff);
	return EXIT_SUCCESS;

}
