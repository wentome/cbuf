#ifndef __BUF_H__
#define __BUF_H__
//#define DEBUG 1
#define BUFSIZE 8
typedef  unsigned char uint8_t;
typedef  unsigned int uint16_t;
void WriteBuf(uint16_t len ,uint8_t *source);
uint16_t ReadBuf(uint16_t len ,uint8_t *target);
#ifdef DEBUG
Debug();
#endif
#endif