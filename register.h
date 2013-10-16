#include <stdint.h>
//register r0 - r7
//
#define R (greg.r)
#define SP (greg.r[6])
#define PC (greg.r[7])
struct GENERAL_REG{
	uint16_t r[8];
	uint16_t work;
}greg;
struct OP_STATUS{
	uint8_t bflag;
	uint8_t mode1;
	uint8_t mode2;
	uint8_t src;
	uint8_t dst;
	uint16_t wsrc;
	uint16_t wdst;
	uint16_t *pwdst;
	uint8_t bsrc[1];
	uint8_t *pbdst[1];
	uint16_t oct_op1;
	uint16_t oct_op2;
	char str_src[25];
	char str_dst[25];
}op_stat;
//prosess status word
struct PSW_REG{
	unsigned short N:1;
	unsigned short Z:1;
	unsigned short V:1;
	unsigned short C:1;
}psw;
struct WORK_DATA{
	char snemonic[64];
	char sop1[64];
	char sop2[64];
	char nbin[64];
	uint8_t mem[0x10000];
}w_data;
struct DEBUG{
	unsigned int break_mem;
	int flag;
}dbg;
