#include<stdint.h>

struct V6_HEAD{
	uint16_t magic;
	uint16_t text;
	uint16_t data;
	uint16_t bss;
	uint16_t symbol_tbl;
	uint16_t ent;
	uint16_t relocation;
}v6_h;
struct V6_SYMBOL{
	char symbol_name[10];
	int type;
	int addr;
}v6_sym[1024];

