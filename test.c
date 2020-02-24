#include "stdio.h"
#include "buf.h"

void main()
{
	uint16_t i=0;
	uint16_t num=0;
	uint8_t source[9]={1,2,3,4,5,6,7,8,9};
	uint8_t source2[9]={11,12,13,14,15,16,17,18,19};
	uint8_t source3[9]={21,22,23,24,25,26,27,28,29};
	uint8_t target[100]={0};
	
	WriteBuf(8,source);
	num=ReadBuf(3,target);
	for (i=0;i<num;i++){
		printf("%2.d|",target[i]);
	}
	printf("\n----------------------\n");
	

	WriteBuf(4,source2);
	num=ReadBuf(10 ,target);
	for (i=0;i<num;i++){
		printf("%2.d|",target[i]);
	}
	printf("\n----------------------\n");
	
	WriteBuf(4,source3);
	num=ReadBuf(10 ,target);
	for (i=0;i<num;i++){
		printf("%2.d|",target[i]);
	}
	printf("\n----------------------\n");
	num=ReadBuf(10 ,target);
	for (i=0;i<num;i++){
		printf("%2.d|",target[i]);
	}
	printf("\n----------------------\n");
	
	
}