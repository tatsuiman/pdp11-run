#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "a.out.h"
#include "register.h"
void usage(char *pname)
{
		printf("%s Usage:\n"
					"  -v <v6 file>\n", pname);
		exit(0);
}

int main(int argc ,char *argv[])
{
	unsigned long tail = 0;
	uint16_t nargc=1;
	int len = 0,i = 0, j = 0, opt;
	uint16_t iargc = 0,pargv[0xFF];
	unsigned char FileName[256];

	dbg.flag = 1;
	for(i = 0;i < 0x10000; i++)w_data.mem[i]=0;
	while((opt=getopt(argc,argv, "dhv:m:"))!=-1){
		switch (opt)
		{
			case 'v':
				iargc = optind - 1;
				strcpy(FileName, optarg);
				tail = ReadFile(FileName, w_data.mem);
				j+=2;
				break;
			case 'm':
				dbg.break_mem = strtol(optarg, NULL, 16);
				j+=2;
				break;
			case 'd':
				dbg.flag = 0;
				j++;
				break;
			case 'h':
				usage(argv[0]);
				break;
			default:
				usage(argv[0]);
				break;
		}
	}//get arg options
	nargc = argc-iargc;
	if(tail <= 0)usage(argv[0]);
	for(i=nargc-1; i>=0; i--){
		SP-=((strlen(argv[iargc+i])+2)&~1);
		strcpy(&w_data.mem[SP],argv[iargc+i]);
		pargv[i] = SP;
	}
	for(i=nargc-1; i>=0; i--){
		SP-=sizeof(uint16_t);
		w_data.mem[SP] = (uint8_t)pargv[i];
		w_data.mem[SP+1] = (uint8_t)(pargv[i]>>8);
	}
	SP -= sizeof(uint16_t);
	w_data.mem[SP] = (uint8_t)nargc;
	w_data.mem[SP+1] = (uint8_t)(nargc>>8);
	//printf("text 0 ~ %X\n", v6_h.text);
	for(PC = 2; PC <= v6_h.text; PC+=2){
		greg.work = PC - 2;
		print_mem();
		Translation(read_word(greg.work));
	}
	return 0;
}
