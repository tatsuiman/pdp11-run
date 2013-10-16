#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "register.h"
#include "a.out.h"

inline uint16_t read_word(uint16_t reg){
	return ((uint16_t)w_data.mem[reg])|((uint16_t)w_data.mem[reg+1]<<8);
}
inline uint32_t read_reg32(uint8_t rnum){
	return (R[rnum]<<16)|R[rnum +1];
}
reg_incrementw(){
	op_stat.bflag = 0;
	if((op_stat.src != 7) && (op_stat.mode1 == 2))R[op_stat.src]+=2;
	if((op_stat.dst != 7) && (op_stat.mode2 == 2))R[op_stat.dst]+=2;
}
reg_incrementb(){
	op_stat.bflag = 1;
	if((op_stat.src != 7) && (op_stat.mode1 == 2))R[op_stat.src]+=1;
	if((op_stat.dst != 7) && (op_stat.mode2 == 2))R[op_stat.dst]+=1;
}
reg_decrementw(){
	op_stat.bflag = 0;
	if((op_stat.src != 7) && (op_stat.mode1 == 4))R[op_stat.src]-=2;
	if((op_stat.dst != 7) && (op_stat.mode2 == 4))R[op_stat.dst]-=2;
}
reg_decrementb(){
	op_stat.bflag = 1;
	if((op_stat.src != 7) && (op_stat.mode1 == 4))R[op_stat.src]-=1;
	if((op_stat.dst != 7) && (op_stat.mode2 == 4))R[op_stat.dst]-=1;
}
init_instruction(){
	uint16_t res = 0;
	greg.work = PC - 2;
	if(op_stat.src == 7){	
		switch(op_stat.mode1){
			case 2:
				op_stat.wsrc = read_word(R[op_stat.src]);
				op_stat.bsrc[0] = w_data.mem[R[op_stat.src]];
				op_stat.bsrc[1] = w_data.mem[R[op_stat.src] + 1];
				break;
			case 3:
				op_stat.wsrc = read_word(read_word(R[op_stat.src]));
				op_stat.bsrc[0] = w_data.mem[read_word(R[op_stat.src])];
				op_stat.bsrc[1] = w_data.mem[read_word(R[op_stat.src]) + 1];
				break;
			case 6:
				op_stat.wsrc = read_word((int16_t)read_word(PC)+PC+2);
				op_stat.bsrc[0] = op_stat.wsrc&0x00FF;
				op_stat.bsrc[1] = op_stat.wsrc>>8;
				break;
			case 7:
				op_stat.wsrc = read_word(read_word((int16_t)read_word(PC)+PC+2));
				op_stat.bsrc[0] = op_stat.wsrc&0x00FF;
				op_stat.bsrc[1] = op_stat.wsrc>>8;
			break;
		}
		PC += 2;
	}else{
		switch(op_stat.mode1){
			case 0:
				op_stat.wsrc = R[op_stat.src];
				op_stat.bsrc[0] = R[op_stat.src];
				op_stat.bsrc[1] = R[op_stat.src] >> 8;
				break;
			case 1:
				op_stat.wsrc = read_word(R[op_stat.src]);
				op_stat.bsrc[0] = w_data.mem[R[op_stat.src]];
				op_stat.bsrc[1] = w_data.mem[R[op_stat.src] + 1];
				break;
			case 2:
				op_stat.wsrc = read_word(R[op_stat.src]);
				op_stat.bsrc[0] = w_data.mem[R[op_stat.src]];
				op_stat.bsrc[1] = w_data.mem[R[op_stat.src] + 1];
				if(op_stat.bflag == 1)R[op_stat.src]+=2;
				else R[op_stat.src]+=1;
				break;
			case 3:
				op_stat.wsrc = read_word(read_word(R[op_stat.src]));
				op_stat.bsrc[0] = w_data.mem[read_word(R[op_stat.src])];
				op_stat.bsrc[1] = w_data.mem[read_word(R[op_stat.src]) + 1];
				break;
			case 4:
				if(op_stat.bflag == 1)R[op_stat.src]-=2;
				else R[op_stat.src]-=1;
				op_stat.wsrc = read_word(R[op_stat.src]);
				op_stat.bsrc[0] = w_data.mem[R[op_stat.src]];
				op_stat.bsrc[1] = w_data.mem[R[op_stat.src] + 1];
				break;
			case 5:
				//@-(r%d)
				break;
			case 6:
				op_stat.wsrc = read_word(R[op_stat.src] + (int16_t)read_word(PC));
				op_stat.bsrc[0] = op_stat.wsrc & 0x00FF;
				op_stat.bsrc[1] = op_stat.wsrc >> 8;
				PC += 2;
				break;
			case 7:
				op_stat.wsrc = read_word(read_word(R[op_stat.src] + (int16_t)read_word(PC)));
				op_stat.bsrc[0] = op_stat.wsrc & 0x00FF;
				op_stat.bsrc[1] = op_stat.wsrc >> 8;
				PC += 2;
				break;
		}
	}
	if(op_stat.dst == 7){
		switch(op_stat.mode2){
			case 2:
				op_stat.wdst = read_word(PC);
				op_stat.pbdst[0] = &w_data.mem[PC];
				op_stat.pbdst[1] = &w_data.mem[PC + 1];
				break;
			case 3:
				op_stat.wdst = read_word(read_word(PC));
				op_stat.pbdst[0] = &w_data.mem[read_word(PC)];//x
				op_stat.pbdst[1] = &w_data.mem[read_word(PC) + 1];//x
				break;
			case 6:
				op_stat.wdst = read_word((int16_t)read_word(PC) + PC +2);
				op_stat.pbdst[0] = &w_data.mem[read_word(PC)+PC + 2];//x
				op_stat.pbdst[1] = &w_data.mem[read_word(PC)+PC + 2 + 1];
				break;
			case 7:
				op_stat.wdst = read_word(read_word((int16_t)read_word(PC) + PC +2));
				op_stat.pbdst[0] = &w_data.mem[read_word(read_word(PC)+PC+2)];//x
				op_stat.pbdst[1] = &w_data.mem[read_word(read_word(PC)+PC+2) + 1];//x
			break;
		}
		PC += 2;
	}else{
		switch(op_stat.mode2){
			case 0:
				op_stat.pwdst = &R[op_stat.dst];
				break;
			case 1:
				op_stat.wdst = read_word(R[op_stat.dst]);
				op_stat.pbdst[0] = &w_data.mem[R[op_stat.dst]];
				op_stat.pbdst[1] = &w_data.mem[R[op_stat.dst] + 1];
				break;
			case 2:
				op_stat.wdst = read_word(R[op_stat.dst]);
				op_stat.pbdst[0] = &w_data.mem[R[op_stat.dst]];
				op_stat.pbdst[1] = &w_data.mem[R[op_stat.dst] + 1];
				break;
			case 3:
				op_stat.wdst = read_word(w_data.mem[R[op_stat.dst]]);
				op_stat.pbdst[0] = &w_data.mem[read_word(R[op_stat.dst])];
				op_stat.pbdst[1] = &w_data.mem[read_word(R[op_stat.dst]) + 1];
				break;
			case 4:
				if(op_stat.bflag == 1)R[op_stat.dst]-=2;
				else R[op_stat.dst]-=1;
				op_stat.wdst = read_word(R[op_stat.dst]);
				op_stat.pbdst[0] = &w_data.mem[R[op_stat.dst]];
				op_stat.pbdst[1] = &w_data.mem[R[op_stat.dst] + 1];
				break;
			case 5:
				//@-(r%d)
				break;
			case 6:
				res = R[op_stat.dst] + (int16_t)read_word(PC);
				op_stat.wdst = read_word(res);
				op_stat.pbdst[0] = &w_data.mem[res];
				op_stat.pbdst[1] = &w_data.mem[res + 1];
				PC += 2;
				break;
			case 7:
				res = read_word(R[op_stat.dst] + (int16_t)read_word(PC));
				op_stat.wdst = read_word(res);
				op_stat.pbdst[0] = &w_data.mem[res];
				op_stat.pbdst[1] = &w_data.mem[res + 1];
				PC += 2;
				break;
		}
	}
}
//op function
mov(){
	print_disas_ssdd("mov");
	reg_decrementw();
	init_instruction();

	if(op_stat.mode2 == 0){//(r | m) -> r
		psw.N = ((int16_t)op_stat.wsrc )< 0;
		psw.Z = op_stat.wsrc == 0;
		*op_stat.pwdst = op_stat.wsrc;
	}else{//(r | m) -> m
		psw.N = (int16_t)op_stat.wsrc < 0;
		psw.Z = op_stat.wsrc == 0;
		*op_stat.pbdst[0] = op_stat.bsrc[0];//Low Bit
		*op_stat.pbdst[1] = op_stat.bsrc[1];//High Bit
	}
	psw.V = 0;
	reg_incrementw();
	return ;
}
movb(){
	print_disas_ssdd("movb");
	reg_decrementb();
	init_instruction();

	if(op_stat.mode2 == 0){//(r | m) -> r
		psw.N = ((int16_t)op_stat.wsrc)&0x00FF< 0;
		psw.Z = (op_stat.wsrc & 0x00FF) == 0;
		*op_stat.pwdst = op_stat.wsrc&0x00FF;
	}else{//(r | m) -> m
		psw.N = (int16_t)op_stat.wsrc < 0;
		psw.Z = ((uint16_t)op_stat.bsrc[0]) == 0;
		*op_stat.pbdst[0] = op_stat.bsrc[0];//Low Bit
	}
	psw.V = 0;
	reg_incrementb();
	return ;
}
cmpb(){
	print_disas_ssdd("cmpb");
	reg_decrementw();
	init_instruction();
	psw.N = (uint16_t)(op_stat.wsrc - op_stat.wdst) < 0;
	psw.Z = op_stat.wsrc == op_stat.wdst;
	psw.V = ((int16_t)(op_stat.wsrc - op_stat.wdst))!=((uint16_t)(op_stat.wsrc - op_stat.wdst));
	psw.C = ((int16_t)op_stat.wsrc < (int16_t)op_stat.wdst);
	reg_incrementw();
}
cmp(){
	print_disas_ssdd("cmp");
	reg_decrementw();
	init_instruction();
	psw.N = (uint16_t)(op_stat.wsrc - op_stat.wdst) < 0;
	psw.Z = op_stat.wsrc == op_stat.wdst;
	psw.V = ((int16_t)(op_stat.wsrc - op_stat.wdst))!=((uint16_t)(op_stat.wsrc - op_stat.wdst));
	psw.C = ((uint16_t)op_stat.wsrc < (uint16_t)op_stat.wdst);
	reg_incrementw();
}
add(){
	int res;
	print_disas_ssdd("add");
	init_instruction();
	int16_t ires =(uint16_t)(*op_stat.pwdst) + (uint16_t)op_stat.wsrc;
	psw.N =(uint16_t)ires < 0;
	psw.Z =(uint16_t)ires == 0;
	psw.V =((uint16_t)ires) != ((int16_t)ires);
	psw.C = (uint16_t)ires >= 0x10000;
	if(op_stat.mode2 == 0){//(r | m) -> r
		//printf(" %d %d",op_stat.wsrc, *op_stat.pwdst);
		*op_stat.pwdst = *op_stat.pwdst + op_stat.wsrc;
	}else{//(r | m) -> m
		res = op_stat.wdst + op_stat.wsrc;
		*op_stat.pbdst[0] = res&0x00FF;//Low Bit
		*op_stat.pbdst[1] = res>>8;//High Bit
	}
}
sub(){
	print_disas_ssdd("sub");
	init_instruction();
	int16_t ires =(int16_t)*op_stat.pwdst - (int16_t)op_stat.wsrc;
	psw.N =(uint16_t)ires < 0;
	psw.Z =(uint16_t)ires == 0;
	psw.V =((uint16_t)ires)!=((int16_t)ires);
	psw.C = *op_stat.pwdst < op_stat.wsrc;
	if(op_stat.mode2 == 0){//(r | m) -> r
		//printf(" %d %d",op_stat.wsrc, *op_stat.pwdst);
		*op_stat.pwdst = *op_stat.pwdst - op_stat.wsrc;
	}else{//(r | m) -> m
		*op_stat.pbdst[0] = (*op_stat.pwdst - op_stat.wsrc)&0x00FF;//Low Bit
		*op_stat.pbdst[1] = (*op_stat.pwdst - op_stat.wsrc)>>8;//High Bit
	}

}
sob(){
	print_src("sob", op_stat.src);
	R[op_stat.src]--;
	if(R[op_stat.src] != 0)PC = PC - (2 * op_stat.oct_op2);
}
asl(){
	print_dst("asl", op_stat.dst);
	op_stat.mode1 = 0;
	init_instruction();
	*op_stat.pwdst = *op_stat.pwdst<<1;
}
ash(){
	print_disas_rss("ash");
	init_instruction();
	int res0 = ((int16_t)R[op_stat.src]);
	if (op_stat.wdst == 0){
		psw.N = res0 < 0;
		psw.Z = res0 == 0;
		psw.V = 0;
		psw.C = psw.C;
	}else{
		if ((op_stat.wdst & 040) == 0)
		{
			int res1 = res0 << (op_stat.wdst - 1);
			int res2 = res1 << 1;
			R[op_stat.src] = res2;
			psw.N = res2 < 0;
			psw.Z = res2 == 0;
			psw.V = res0 != res2;
			psw.C = (res1 & 1) != 0;
		}
		else
		{
			int res1 = res0 >> (63 - op_stat.wdst);
			int res2 = res1 >> 1;
			R[op_stat.src] = res2;
			psw.N = res2 < 0;
			psw.Z = res2 == 0;
			psw.V = res0 != res2;
			psw.C = res1 < 0;
		}
	}
}
bic(){
	print_disas_ssdd("bic");
	init_instruction();
	int16_t res0 = ~op_stat.wsrc&(op_stat.wdst);
	if(op_stat.mode2 == 0){
		*op_stat.pwdst = ~op_stat.wsrc&(*op_stat.pwdst);
		psw.N = (res0&0x8000) != 0;
		psw.Z = res0 == 0;
		psw.V = 0;
	}else{
		*op_stat.pbdst[0] = (~op_stat.wsrc&(op_stat.wdst))&0x00FF;
		*op_stat.pbdst[1] = (~op_stat.wsrc&(op_stat.wdst))>>8;
		psw.N = (res0&0x8000) != 0;
		psw.Z = res0 == 0;
		psw.V = 0;
	}
}

inc(){
	print_disas_dd("inc");
	op_stat.mode1 = 0;
	init_instruction();
	psw.N = *op_stat.pwdst+1 < 0;
	psw.Z = *op_stat.pwdst+1 == 0;
	psw.V = *op_stat.pwdst+1 == 0x8000;
	if(op_stat.mode2 == 0){
		*op_stat.pwdst = *op_stat.pwdst + 1;
	}else{
		*op_stat.pbdst[0] = (op_stat.wdst + 1)&0x00FF;
		*op_stat.pbdst[1] = (op_stat.wdst + 1)>>8;
	}
}
dec(){
	print_disas_dd("dec");
	op_stat.mode1 = 0;
	init_instruction();
	psw.N = op_stat.wdst-1 < 0;
	psw.Z = op_stat.wdst-1 == 0;
	psw.V = op_stat.wdst-1 == -0x8001;

	if(op_stat.mode2 == 0)
		op_stat.pwdst--;
	else{
		*op_stat.pbdst[0] = (op_stat.wdst - 1)&0x00FF;
		*op_stat.pbdst[1] = (op_stat.wdst - 1)>>8;
	}
}
bgt(){
	if(dbg.flag != 0)printf("bgt");
	if((psw.Z|(psw.N^psw.V)) == 0)PC = PC + (2 * w_data.mem[PC-1]);
}
jsr(){
	print_disas_rdd("jsr");
	SP-=2;//push
	PC += 2;
	w_data.mem[SP] = (uint8_t)R[op_stat.src];
	w_data.mem[SP+1] = (uint8_t)(R[op_stat.src]>>8);
	R[op_stat.src] = PC;
	
	if(op_stat.dst == 7){
		switch(op_stat.mode2){
			case 2:
				//(r%d)+
				break;
			case 3:
				PC = read_word(PC - 2);
				break;
			case 6:
				PC = read_word(PC - 2) + PC;
				break;
			case 7:
				//@X(r%d)
			break;
		}
	}
}
rts(){
	print_dst("rts", op_stat.dst);
	PC = R[op_stat.dst];
	R[op_stat.dst] = read_word(SP);//pop
	SP += 2;
}
jmp(){
	print_disas_dd("jmp");
	op_stat.mode1 = 0;
	reg_decrementw();
	if (op_stat.mode2 == 6){
		PC += (read_word(PC)+2);
	}else{
		PC = R[op_stat.dst];
	}
	reg_incrementw();
}
bhi(){
	if(dbg.flag != 0)printf("bhi");
	if (psw.C == 0 && psw.Z == 0 )PC = PC + (2 * (int8_t)(w_data.mem[PC-2]));
}
bge(){
	if(dbg.flag != 0)printf("bge");
	if (psw.V == 0 || psw.N == 0 )PC = PC + (2 * (int8_t)(w_data.mem[PC-2]));
}
bcc(){
	if(dbg.flag != 0)print_disas("bcc");
	if(psw.C == 0){
		PC = PC + (2 * (int8_t)(w_data.mem[PC-2]));
	}
}
bne(){
	if(dbg.flag != 0)printf("bne");
	if(psw.Z == 0)PC = PC + (2 * (int8_t)(w_data.mem[PC-2]));
}
ble(){
	if(dbg.flag != 0)printf("ble");
	if((psw.Z | (psw.N ^ psw.V)) == 1)PC = PC + (2 * (int8_t)(w_data.mem[PC-2]));
}
br(){
	if(dbg.flag != 0)printf("br");
	PC = PC + (2 * (int8_t)(w_data.mem[PC-2]));
}

beq(){
	if(dbg.flag != 0)printf("beq");
	if(psw.Z == 1)PC = (uint16_t)(PC + (2 * (int8_t)(w_data.mem[PC-2])));
}
neg(){
	print_disas_dd("neg");
	op_stat.mode1 = 0;
	op_stat.src = 0;
	reg_decrementw();
	if(op_stat.mode2 == 0){
		psw.N = -((int16_t)(R[op_stat.dst])) < 0;
		psw.Z = R[op_stat.dst] == 0;
		psw.V = -((int16_t)(R[op_stat.dst])) == 8000;
		psw.C = !psw.Z;
		R[op_stat.dst] = ~R[op_stat.dst] + 1;
	}else{
		*op_stat.pbdst[0] = -((int16_t)(w_data.mem[R[op_stat.dst]])) & 0x00FF;
		*op_stat.pbdst[1] = -((int16_t)(w_data.mem[R[op_stat.dst]])) >> 8;
		psw.N = -((int16_t)(w_data.mem[R[op_stat.dst]])) < 0;
		psw.Z = w_data.mem[R[op_stat.dst]] == 0;
		psw.V = -((int16_t)(w_data.mem[R[op_stat.dst]])) == 8000;
		psw.C = !psw.Z;
	}
	reg_incrementw();

}
tstb(){
	print_disas_dd("tstb");
	op_stat.mode1 = 0;
	op_stat.src = 0;
	init_instruction();
	reg_decrementb();
	if(op_stat.mode2 == 0){
		psw.N = (int16_t)(R[op_stat.dst]&0x00FF)< 0;
		psw.Z = (R[op_stat.dst]&0x00FF) == 0;
		psw.V = 0;
		psw.C = 0;
	}else{
		psw.N = (int16_t)w_data.mem[R[op_stat.dst]] < 0;
		psw.Z = w_data.mem[R[op_stat.dst]] == 0;
		psw.V = 0;
		psw.C = 0;
	}
	reg_incrementb();
}

tst(){
	print_disas_dd("tst");
	op_stat.mode1 = 0;
	op_stat.src = 0;
	reg_decrementw();
	init_instruction();
	if(op_stat.mode2 == 0){
		psw.N = (int16_t)op_stat.wdst < 0;
		psw.Z = op_stat.wdst == 0;
		psw.V = 0;
		psw.C = 0;
	}else{
		psw.N = (int16_t)op_stat.wdst < 0;
		psw.Z = op_stat.wdst == 0;
		psw.V = 0;
		psw.C = 0;
	}
	reg_incrementw();
}
_div(){
	init_instruction();
	print_disas_rss("div");
	if (op_stat.wdst == 0 || abs((int16_t)R[op_stat.src]) > abs(op_stat.pwdst)){
		psw.N = 0;
		psw.Z = 0;
		psw.V = 1;
		psw.C = op_stat.wdst == 0;
	}else{
		uint32_t reg32 = read_reg32(op_stat.src);
		int32_t rv1 = reg32/op_stat.wdst;
		R[op_stat.src] = rv1;
		R[op_stat.src + 1] = reg32%op_stat.wdst;
		psw.N = rv1 < 0;
		psw.Z = rv1 == 0;
		psw.V = 0;
		psw.C = 0;
	}
}
clr(){
	print_disas_dd("clr");
	op_stat.mode1 = 0;
	init_instruction();
	reg_decrementw();
	if(op_stat.mode2 == 0)
		R[op_stat.dst] = 0;
	else{
		*op_stat.pbdst[0] = 0;
		*op_stat.pbdst[1] = 0;
	}
	psw.N = 0;
	psw.Z = 1;
	psw.V = 0;
	psw.C = 0;
	reg_incrementw();
}
char *readstr(uint16_t index){
	return &w_data.mem[index];
}
sys(uint16_t addr, uint8_t ref){
	uint8_t sys_call_num = w_data.mem[addr - 2];
	if(dbg.flag != 0)printf("sys ");
	switch (sys_call_num){
		case 0:
			if(dbg.flag != 0)printf("indir; %04x\n", read_word(addr));
			print_mem();print_register();
			sys(read_word(addr)+2, 1);
			PC += 2;
			break;
		case 1:
			if(dbg.flag != 0)puts("exit");
			exit(R[0]);
			break;
		case 4:
			if(dbg.flag != 0)printf("write; %04x; %04x\n", read_word(addr),read_word(addr+2));
			fflush(stdout);
			R[0]  = write(R[0], &w_data.mem[read_word(addr)],
					read_word(addr+2));
			if(ref == 0)PC+=4;
			break;
		case 5:
			if(dbg.flag != 0)printf("open; \"%s\"; %x\n", readstr(read_word(addr)),read_word(addr+2));
			R[0] = open(readstr(read_word(addr)), read_word(addr+2));
			break;
		default :
		printf("NI %X\n", w_data.mem[addr-2]);
	}
}
int Translation(uint16_t disas_code){
	uint16_t mnemonic = disas_code >> 12;
	op_stat.mode1 = (disas_code >> 9) & 7;
	op_stat.mode2 = (disas_code >> 3) & 7;
	op_stat.src = (disas_code >> 6) & 7;
	op_stat.dst = disas_code & 7;
	op_stat.oct_op1 = (op_stat.src|(op_stat.mode1<<3));
	op_stat.oct_op2 = (op_stat.dst|(op_stat.mode2<<3));
	if(dbg.flag != 0)print_register();
	//selectSymbol(greg.work);//show symbols
	switch (mnemonic) {
		case 00:
			if(op_stat.oct_op1 == 0){
				switch (op_stat.oct_op2){
					case 06:
						//rtt
						break;
				}
			}else if(op_stat.oct_op1 == 1)jmp();
			else if(op_stat.oct_op1 == 2)rts();
			else if((op_stat.oct_op1 >= 4) && (op_stat.oct_op1 <= 7))br();
			else if((op_stat.oct_op1 >= 8) && (op_stat.oct_op1 <= 11))bne();//printf("NI %06o",disas_code);
			else if((op_stat.oct_op1 >= 12) && (op_stat.oct_op1 <= 15))beq();
			else if(op_stat.oct_op1 == 16)bge();
			else if(op_stat.oct_op1 == 20)printf("NI %06o",disas_code);//blt
			else if(op_stat.oct_op1 == 24)bgt();
			else if(op_stat.oct_op1 == 28)ble();
			else if(op_stat.oct_op1 == 40)clr();
			else if(op_stat.oct_op1 == 41)printf("NI %06o",disas_code);//com
			else if(op_stat.oct_op1 == 42)inc();
			else if(op_stat.oct_op1 == 43)dec();
			else if(op_stat.oct_op1 == 44)neg();//neg
			else if(op_stat.oct_op1 == 45)printf("NI %06o",disas_code);//adc
			else if(op_stat.oct_op1 == 46)printf("NI %06o",disas_code);//sbc
			else if(op_stat.oct_op1 == 47)tst();
			else if(op_stat.oct_op1 == 51)asl();
			else if(op_stat.mode1 == 4)jsr();
			else printf("NI %06o",disas_code);
			break;
		case 01:
			mov();
			break;
		case 02: 
			cmp();
			break;
		case 04:
			bic();
			break;
		case 06: 
			add();
			break;
		case 07: 
			if((op_stat.oct_op1 >= 8) && (op_stat.oct_op1 < 16))_div();//div();
			else if((op_stat.oct_op1 >= 16) &&(op_stat.oct_op1 <= 23))ash();
			else if((op_stat.oct_op1 >= 56) &&(op_stat.oct_op1 <= 63))sob();
			else printf("NI %06o",disas_code);
			break;
		case 010:
			if (op_stat.oct_op1 == 8)bhi();
			if (op_stat.oct_op1 > 23 && op_stat.oct_op1 < 28)bcc();
			if (op_stat.oct_op1 > 35 && op_stat.oct_op1 < 40){
				sys(PC, 0);
				return 0;
			}
			if(op_stat.oct_op1 == 47)tstb();
			break;
		case 011: 
			movb();//movb
			break;
		case 012: 
			cmpb();
			break;
		case 013: 
			printf("NI %06o",disas_code);//bitb
			break;
		case 014: 
			printf("NI %06o",disas_code);//bicb
			break;
		case 015: 
			printf("NI %06o",disas_code);//bisb
			break;
		case 016:
			sub();
			break;
		case 017:
			print_disas("setd");
			break;
		default:
			printf("NI %06o",disas_code);//.word
			break;
	}
	if(dbg.flag != 0)print_bin();
	return 0;
}
