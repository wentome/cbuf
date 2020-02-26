#ifndef __BUF_H__
#define __BUF_H__
#define DEBUG 1
typedef  unsigned char uint8_t;
typedef  unsigned int uint16_t;
struct CBuf {
	uint16_t wp;
	uint16_t rp;
	uint8_t overFlow;
	uint16_t bufSize;
	uint8_t *buffer;
};
struct CBuf *NewBuf(uint16_t bufSize);
void WriteBuf(struct CBuf *cbuf,uint16_t len ,uint8_t *source);
uint16_t ReadBuf(struct CBuf *cbuf,uint16_t len ,uint8_t *target);
uint16_t GetRestSize(struct CBuf *cbuf);
uint16_t CopyBuf(struct CBuf *cbuf,uint16_t offsetP, uint16_t len ,uint8_t *target);
#ifdef DEBUG
void Debug(struct CBuf *cbuf);
#endif
#endif