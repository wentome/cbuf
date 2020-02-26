#include "stdlib.h"
#include "stdio.h"
#include "buf.h"

#ifdef DEBUG
void Debug(struct CBuf *cbuf){
	uint16_t i =0;
	for(i=0;i<cbuf->bufSize;i++){
		if (i==cbuf->wp){
			if (cbuf->rp==cbuf->wp){
				printf(" # ");
			}else{
				printf(" w ");
			}
			
		}else if(i==cbuf->rp){
			if (cbuf->rp==cbuf->wp){
				printf(" # ");
			}else{
				printf(" r ");
			}
		}else{
			printf("   ");
		}
	}
	printf("\n");
	for(i=0;i<cbuf->bufSize;i++){
		printf("%2.d|",cbuf->buffer[i]);
	}
	printf("r:%2.d w%2.d over:%d|\n", cbuf->rp,cbuf->wp,cbuf->overFlow);
}
#endif
struct CBuf* NewBuf(uint16_t bufSize){
	struct CBuf *cbuf;
	cbuf = (struct CBuf *)malloc(sizeof(struct CBuf));
	cbuf->wp =0;
	cbuf->rp =0;
	cbuf->overFlow=0;
	cbuf->bufSize =bufSize;
	cbuf->buffer = malloc(bufSize);
	return cbuf;
}
//跨界cbuf->wp=cbuf->rp -> cbuf->wp>cbuf->rp
void WriteBuf(struct CBuf *cbuf,uint16_t len ,uint8_t *source){
	#ifdef DEBUG
	    printf("\n");
		Debug(cbuf);
		printf("W:%d\n",len);
	#endif
	
	uint16_t i=0;
	uint16_t resetToEndSize=0;
	
	if (len>cbuf->bufSize){
		source=source+len-cbuf->bufSize;
		len =cbuf->bufSize;
	}
	// 一定full
	if (cbuf->rp>cbuf->wp){
		if (len>cbuf->rp-cbuf->wp){
			cbuf->overFlow=1;
		}
	}else{
		if(len>=cbuf->bufSize-cbuf->wp+cbuf->rp){
			cbuf->overFlow=1;
		}
	}
	resetToEndSize=cbuf->bufSize-cbuf->wp;
	if (resetToEndSize>len){
		for (i=0;i<len;i++){
			*(cbuf->buffer+cbuf->wp+i)=*(source+i);
		}
		cbuf->wp+=len;
	}else{
		//一定大于返回0
		for (i=0;i<resetToEndSize;i++){
			*(cbuf->buffer+cbuf->wp+i)=*(source+i);
		}
		for (i=0;i<len-resetToEndSize;i++){
			*(cbuf->buffer+i)=*(source+resetToEndSize+i);
		}
		cbuf->wp=len-resetToEndSize;
	}
	#ifdef DEBUG
	Debug(cbuf);
	#endif
}

uint16_t ReadBuf(struct CBuf *cbuf, uint16_t len, uint8_t *target){
	#ifdef DEBUG
	printf("\n");
	Debug(cbuf);
    printf("R:%d\n",len);
	#endif
	uint16_t i=0;
	uint16_t resetSize=0;
	uint16_t readSize=0;
	//如果发生溢出 cbuf->wp 跨越 cbuf->rp
	if (cbuf->overFlow==1){
		cbuf->rp=cbuf->wp;	
	}
	//没有溢出cbuf->rp
	if (cbuf->rp<cbuf->wp){
		resetSize=cbuf->wp-cbuf->rp;
		if (len>resetSize){
			len=resetSize;
		}
		for(i=0;i<len;i++){
			*(target+i)=*(cbuf->buffer+cbuf->rp+i);
		}
		cbuf->rp+=len;
		#ifdef DEBUG
		Debug(cbuf);
		#endif
		return len;
	// 溢出相等 或者 读完数据相等 或者 只是cbuf->wp<cbuf->rp
	}else {
        if (cbuf->rp==cbuf->wp){
			if(cbuf->overFlow==1){//溢出相等
				resetSize=cbuf->bufSize;
				cbuf->overFlow=0;
			}else{//读完数据相等
				#ifdef DEBUG
				Debug(cbuf);
				#endif
				return 0;
			}
		}else{//只是cbuf->wp<cbuf->rp
			resetSize=cbuf->bufSize-cbuf->rp+cbuf->wp;
		}
		
		
		if (len>resetSize){
			len=resetSize;
		}
		
		if(len<cbuf->bufSize-cbuf->rp){
			for(i=0;i<len;i++){
				*(target+i)=*(cbuf->buffer+cbuf->rp+i);
			}
			cbuf->rp+=len;
			#ifdef DEBUG
			Debug(cbuf);
			#endif
			return len;
		}else{
			readSize=cbuf->bufSize-cbuf->rp;
			for(i=0;i<readSize;i++){
				*(target+i)=*(cbuf->buffer+cbuf->rp+i);
			}
			cbuf->rp=0;
			for(i=0;i<cbuf->wp;i++){
				*(target+readSize+i)=*(cbuf->buffer+i);
			}
			cbuf->rp+=cbuf->wp;
			#ifdef DEBUG
			Debug(cbuf);
			#endif
			return len;
		}
		
		
	}
}

uint16_t GetRestSize(struct CBuf *cbuf)
{
	uint16_t resetSize=0;
	if (cbuf->overFlow==1){
		resetSize=cbuf->bufSize;
	}else{
		if (cbuf->rp<cbuf->wp){
			resetSize=cbuf->wp-cbuf->rp;
		}else if (cbuf->rp==cbuf->wp){
			resetSize=0;
		}else{
			resetSize=cbuf->bufSize-cbuf->rp+cbuf->wp;
		}
	}
	return resetSize;	
}
//返回有效的数据个数	
uint16_t CopyBuf(struct CBuf *cbuf,uint16_t offsetP, uint16_t len ,uint8_t *target)
{
	uint16_t i=0;
	uint16_t readSize=0;
	uint16_t copyP=0;
	uint16_t resetSize=0;
	resetSize=GetRestSize(cbuf);
	//正真可读的len 确保len有效
	if (len>resetSize-offsetP){
		len=resetSize-offsetP;
	}
	//获取copy 实际位置
	if(cbuf->rp+offsetP<cbuf->bufSize){
		copyP=cbuf->rp+offsetP;
	}else{
		copyP=offsetP-(cbuf->bufSize-cbuf->rp);
	}
	//最简单的直接读取
	if (len<cbuf->bufSize-copyP){
		for(i=0;i<len;i++){
				*(target+i)=*(cbuf->buffer+copyP+i);
		}
	}else{
		readSize=cbuf->bufSize-copyP;
		for(i=0;i<readSize;i++){
			*(target+i)=*(cbuf->buffer+copyP+i);
		}
		for(i=0;i<len-readSize;i++){
			*(target+readSize+i)=*(cbuf->buffer+i);
		}	
	}
	return len;
}

