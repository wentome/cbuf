#include "stdio.h"
#include "buf.h"

uint8_t Buffer[BUFSIZE]={0};
uint16_t wp =0;
uint16_t rp =0;
uint8_t overflow=0;
#ifdef DEBUG
void Debug(){
	uint16_t i =0;
	for(i=0;i<BUFSIZE;i++){
		if (i==wp){
			if (rp==wp){
				printf(" # ");
			}else{
				printf(" w ");
			}
			
		}else if(i==rp){
			if (rp==wp){
				printf(" # ");
			}else{
				printf(" r ");
			}
		}else{
			printf("   ");
		}
	}
	printf("\n");
	for(i=0;i<BUFSIZE;i++){
		printf("%2.d|",Buffer[i]);
	}
	printf("r:%2.d w%2.d over:%d|\n", rp,wp,overflow);
}
#endif
//跨界wp=rp -> wp>rp
void WriteBuf(uint16_t len ,uint8_t *source){
	#ifdef DEBUG
	    printf("\n");
		Debug();
		printf("W:%d\n",len);
	#endif
	
	uint16_t i=0;
	uint16_t resetToEndSize=0;
	
	if (len>BUFSIZE){
		source=source+len-BUFSIZE;
		len =BUFSIZE;
	}
	// 一定full
	if (rp>wp){
		if (len>rp-wp){
			overflow=1;
		}
	}else{
		if(len>=BUFSIZE-wp+rp){
			overflow=1;
		}
	}
	resetToEndSize=BUFSIZE-wp;
	if (resetToEndSize>len){
		for (i=0;i<len;i++){
			*(Buffer+wp+i)=*(source+i);
		}
		wp+=len;
	}else{
		//一定大于返回0
		for (i=0;i<resetToEndSize;i++){
			*(Buffer+wp+i)=*(source+i);
		}
		for (i=0;i<len-resetToEndSize;i++){
			*(Buffer+i)=*(source+resetToEndSize+i);
		}
		wp=len-resetToEndSize;
	}
	#ifdef DEBUG
	Debug();
	#endif
}

uint16_t ReadBuf(uint16_t len ,uint8_t *target){
	#ifdef DEBUG
	printf("\n");
	Debug();
    printf("R:%d\n",len);
	#endif
	uint16_t i=0;
	uint16_t resetSize=0;
	uint16_t readSize=0;
	//如果发生溢出
	if (overflow==1){
		rp=wp;	
	}
	//没有溢出
	if (rp<wp){
		resetSize=wp-rp;
		if (len>resetSize){
			len=resetSize;
		}
		for(i=0;i<len;i++){
			*(target+i)=*(Buffer+rp+i);
		}
		rp+=len;
		#ifdef DEBUG
		Debug();
		#endif
		return len;
	// 溢出相等 或者 读完数据相等 或者 只是wp<rp
	}else {
        if (rp==wp){
			if(overflow==1){//溢出相等
				resetSize=BUFSIZE-rp+wp;
				overflow=0;
			}else{//读完数据相等
				#ifdef DEBUG
				Debug();
				#endif
				return 0;
			}
		}else{//只是wp<rp
			resetSize=BUFSIZE-rp+wp;
		}
		
		
		if (len>resetSize){
			len=resetSize;
		}
		
		if(len<BUFSIZE-rp){
			for(i=0;i<len;i++){
				*(target+i)=*(Buffer+rp+i);
			}
			rp+=len;
			#ifdef DEBUG
			Debug();
			#endif
			return len;
		}else{
			readSize=BUFSIZE-rp;
			for(i=0;i<readSize;i++){
				*(target+i)=*(Buffer+rp+i);
			}
			rp=0;
			for(i=0;i<wp;i++){
				*(target+readSize+i)=*(Buffer+i);
			}
			rp+=wp;
			#ifdef DEBUG
			Debug();
			#endif
			return len;
		}
		
		
	}
}

