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
    struct CBuf *cbuf1;
	struct CBuf *cbuf2;
	cbuf1=NewBuf(8);
	cbuf2=NewBuf(8);
	//printf("buf:%d\n",cbuf1->bufSize);
	
	WriteBuf(cbuf1,2,source);
	WriteBuf(cbuf2,2,source2);
	num=GetRestSize(cbuf1);
	printf("RestSize：%2.d\n",num);
	num=ReadBuf(cbuf1,3,target);
	for (i=0;i<num;i++){
		printf("%2.d|",target[i]);
	}
	printf("\n----------------------\n");
	num=ReadBuf(cbuf1,3,target);
	for (i=0;i<num;i++){
		printf("%2.d|",target[i]);
	}
	printf("\n----------------------\n");
	num=ReadBuf(cbuf1,3,target);
	for (i=0;i<num;i++){
		printf("%2.d|",target[i]);
	}
	printf("\n---------2-------------\n");
	
	
	WriteBuf(cbuf2,2,source);
	WriteBuf(cbuf2,2,source2);
	num=GetRestSize(cbuf2);
	printf("RestSize：%2.d\n",num);
	num=ReadBuf(cbuf2,3,target);
	for (i=0;i<num;i++){
		printf("%2.d|",target[i]);
	}
	printf("\n----------------------\n");
	num=ReadBuf(cbuf2,3,target);
	for (i=0;i<num;i++){
		printf("%2.d|",target[i]);
	}
	printf("\n----------------------\n");
	num=ReadBuf(cbuf2,3,target);
	for (i=0;i<num;i++){
		printf("%2.d|",target[i]);
	}
	printf("\n----------------------\n");
}