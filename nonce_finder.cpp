#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <conio.h>
#include "blake2s.h"

using namespace std;

#define BLOCK_LEN	286
#define HASH_LEN	32
#define NONCE_LEN	8

uint8_t block_data[BLOCK_LEN]; 
uint8_t target_hash[HASH_LEN];
uint8_t nonce[NONCE_LEN];

//void print_hash//
bool CompareHash(const uint8_t* first, const uint8_t* second) {
	
	int i = HASH_LEN;
	while(--i)
	{
		if(first[i]!=second[HASH_LEN-i-1])
			return false;
	}
	return true;
}

#define hexchar2int(ch) (ch>='a' && ch<='f') ? (ch - 'a' + 10) : (ch-'0')

void read_BlockData(const char *str, uint8_t *target)
{
    for (size_t i = 0; i < BLOCK_LEN; i++)
    {
        uint8_t c1 = hexchar2int(str[2 * i]);
        uint8_t c2 = hexchar2int(str[2 * i + 1]);
        target[i] = (c1 << 4) | c2;
    }
}

int charStrToByte(char* pStr, size_t pStrLenght, uint8_t* pByte)
{
	uint32_t len = pStrLenght / 2;
	uint32_t i, j;
	for (i = 0, j = 0; i < len; i++, j += 2)
	{
		{
			//char* msg = afe + pStr.substr(j, 2);
			char msg[5];
			sprintf(msg, "0x%c%c", pStr[j], pStr[j + 1]);
			int number = 0;
			sscanf(msg, "0x%X", &number);
			pByte[i] = number;
		}
	}
	return len;
}

void reverse_memcpy(uint8_t *dest, uint8_t * source, uint16_t len)
{
	for(int i=0; i<len; i++)
		dest[len-i-1] = source[i];
}

void increase(uint8_t* data, uint16_t len)
{
	int i=len-1;
	bool flag= true;
	
	while(i>=0)
	{
		if(data[i]!=0xff)
			flag = true;
		else
			flag = false;
		
		data[i]++;
		if(flag)
			break;
		i--;
	}
}

void print_byte_series(const char *title, uint8_t *data, uint16_t len)
{
	printf("\n##################### = %s = #####################\n", title);
	for(int i=len-1; i>=0; i--)
	{
		printf("0x%02x ", data[i]);
	}
	printf("\n");
}


bool FindRightNonce()
{
	uint8_t finalblockHash[32];
	uint8_t	cur_block[BLOCK_LEN];
	uint8_t out_hash[HASH_LEN];
	uint64_t index=0;

	memcpy(cur_block, block_data, BLOCK_LEN-NONCE_LEN);
	while (1) // it loops forever until result became  different from -1.
	{
		memcpy(&cur_block[BLOCK_LEN-NONCE_LEN], nonce, NONCE_LEN);
		
		blake2s(out_hash, HASH_LEN , NULL, 0, cur_block, BLOCK_LEN);

		index++;
//		if(index%10000000==0)
//		{
//			printf("\n  ##   Number %d\n", index);
//			print_byte_series("   Nonce   ", nonce, NONCE_LEN);
//			
//		}

		printf("\n  ##   Number %d\n", index);	
		print_byte_series("Solved Hash", out_hash, HASH_LEN);
		print_byte_series("   Nonce   ", nonce, NONCE_LEN);
		
		if(CompareHash(out_hash, target_hash))
		{
			printf("\n  ##   Number %d\n", index);
			printf("\n\n\nCongratulations, we founded the golden nonce\n");
			print_byte_series(" Golden  Nonce   ", nonce, NONCE_LEN);

			return true;
		}
		else
		{
			increase(nonce, NONCE_LEN);
		}
		
	}
	return false;
}


bool input_data()
{

    char str[2000];

    freopen("in.txt","r", stdin);
    scanf("%s",str);
	

	printf("##################### Block Data =  #####################\n\n");
	
	if(strlen(str) != 2*BLOCK_LEN)
	{
		printf("Input Block Error\n");
		return false;
	}
	// Display block_data
	for(int i=0; i<BLOCK_LEN; i++)
	{
		printf("0x%c%c ",str[2*i], str[2*i+1]);
		if((i+1)%16 == 0) printf("\n");
	}
	printf("\n");
	
	read_BlockData(str, block_data);
	
	
	
	
	printf("##################### Target Hash =  #####################\n\n");

		// Get TargetHash
	memcpy(target_hash, &block_data[158], 32);
	
print_byte_series("Target Hash", target_hash, HASH_LEN);

//	 scanf("%s",str);
//	
//	
//	 if(strlen(str) != 2*HASH_LEN)
//	 {
//	 	printf("Target Hash Error\n");
//	 	return false;
//	 }
//
//	 read_BlockData(str, target_hash);
	 for(int i=0; i<HASH_LEN; i++)
	 	printf("%02x ",target_hash[i]);
	 printf("\n\n");
   


	
	
	// Get Nonce
	memcpy(nonce, &block_data[278], 8);
	print_byte_series("   Nonce   ", nonce, NONCE_LEN);
	
	printf("\n");

}

int main(int argc, char **argv)
{
	if(!input_data()) return 0;
	FindRightNonce();
	
    return 0;
}
