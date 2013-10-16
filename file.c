#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "a.out.h"

int v6_sym_array_num = 0;
int read_magic(char *FileName){
	int i = 0;
	unsigned char header[16];
	FILE *fp = fopen(FileName, "rb");
	if( fp == NULL ) return -1;
	fread(header,sizeof(header),1,fp);
	v6_h.magic = header[0]|(header[1]<<8);
	v6_h.text	=	header[2]|(header[3]<<8);
	v6_h.data	=	header[4]|(header[5]<<8);
	v6_h.bss	=	header[6]|(header[7]<<8);
	v6_h.symbol_tbl = header[8]|(header[9]<<8);
	v6_h.ent	=	header[10]|(header[11]<<8);
	v6_h.relocation = header[14]|(header[15]<<8);
	return 0;
}
//file open and read first line
long ReadFile(char *FileName, unsigned char *code){
	read_magic(FileName);
	uint16_t readlen = 0, ret=0;
	FILE *fp = fopen(FileName, "rb");
	if( fp == NULL ) return ret;
	fseek(fp, 16L, SEEK_SET);
	readlen = v6_h.text + v6_h.data;
	ret = fread(code,1,readlen,fp);
	loadSymbol(code, ftell(fp)-16);
	return ret;
}
int loadSymbol(unsigned char *p_code, long file_size){
	int start = 0,i = 0,j = 0;
	start = v6_h.text + v6_h.data;
	for(i = start, j = 0; i < file_size; i+=12,j++){
		strcpy(v6_sym[j].symbol_name ,&p_code[i]);
		v6_sym[j].symbol_name[8] = '\0';
		v6_sym[j].type = p_code[i + 8]|p_code[i + 9] << 8;
		v6_sym[j].addr = p_code[i + 10]|p_code[i + 11] << 8;
		v6_sym_array_num++;
	}
}
int selectSymbol(int v6_sym_addr){
	int i = 0, j = 0;
	for(j = 0; j < v6_sym_array_num; j++){
		if(v6_sym_addr == v6_sym[j].addr){
			switch(v6_sym[j].type){
				case 0x0024:
					printf("%07oB  %s\n", v6_sym[j].addr, v6_sym[j].symbol_name);
					break;
				case 0x0022:
					printf("%07oT %s\n", v6_sym[j].addr, v6_sym[j].symbol_name);
					break;
				case 0x0002:
					printf("%07ot %s\n", v6_sym[j].addr, v6_sym[j].symbol_name);
					break;
				case 0x0003:
					printf("%07od %s\n", v6_sym[j].addr, v6_sym[j].symbol_name);
					break;
				case 0x0004:
					printf("%07ob %s\n", v6_sym[j].addr, v6_sym[j].symbol_name);
					break;
				case 0x001F:
					printf("%07oa %s\n", v6_sym[j].addr, v6_sym[j].symbol_name);
					break;
				default:
					printf("%07o? %s\n", v6_sym[j].addr, v6_sym[j].symbol_name);
					break;
			}
		}
	}
}
