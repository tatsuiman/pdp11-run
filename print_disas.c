#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "register.h"
#include "a.out.h"
print_mem(){
	if(dbg.break_mem != 0){
		printf("[0x%04x] [%02x%02x] | ",
			dbg.break_mem, w_data.mem[dbg.break_mem+1],
			w_data.mem[dbg.break_mem]);
	}
}
print_bin(){
	if(dbg.flag != 0)puts("");
	return;
	printf(" |%04X %04X %04X %04X\n", 
			read_word(PC-2),
			read_word(PC),
			read_word(PC+2),
			read_word(PC+4));
}
init_str_src(){
	if(op_stat.src == 7){	
		switch(op_stat.mode1){
			case 2:
				sprintf(op_stat.str_src, "$%04x", read_word(greg.work+2));
				break;
			case 3:
				sprintf(op_stat.str_src, "*$%04x", read_word(greg.work+2));
				break;
			case 6:
				sprintf(op_stat.str_src, "%04x", (greg.work+4)+read_word(greg.work+2));
				break;
			case 7:
				sprintf(op_stat.str_src, "*%04x", read_word(greg.work+2)+(greg.work+4));
			break;
		}
		greg.work += 2;
	}else{
		switch(op_stat.mode1){
			case 0:
				sprintf(op_stat.str_src, "r%d", op_stat.src);
				break;
			case 1:
				sprintf(op_stat.str_src, "(r%d)", op_stat.src);
				break;
			case 2:
				sprintf(op_stat.str_src, "(r%d)+", op_stat.src);
				break;
			case 3:
				sprintf(op_stat.str_src, "@(r%d)+", op_stat.src);
				break;
			case 4:
				sprintf(op_stat.str_src, "-(r%d)", op_stat.src);
				break;
			case 5:
				sprintf(op_stat.str_src, "@-(r%d)", op_stat.src);
				break;
			case 6:
				sprintf(op_stat.str_src, "%d(r%d)", (int16_t)read_word(PC), op_stat.src);
				greg.work += 2;
				break;
			case 7:
				sprintf(op_stat.str_src, "*%d(r%d)", (int16_t)read_word(PC), op_stat.src);
				greg.work += 2;
				break;
		}
	}
}
init_str_dst(){
	if(op_stat.dst == 7){	
		switch(op_stat.mode2){
			case 2:
				sprintf(op_stat.str_dst, "$%04x", read_word(greg.work+2));
				break;
			case 3:
				sprintf(op_stat.str_dst, "*$%04x", read_word(greg.work+2));
				break;
			case 6:
				sprintf(op_stat.str_dst, "%04x", 0xFFFF & ((greg.work+4)+read_word(greg.work+2)));
				break;
			case 7:
				sprintf(op_stat.str_dst, "*%04x", (greg.work+4)+read_word(greg.work+2));
			break;
		}
	}else{
		switch(op_stat.mode2){
			case 0:
				sprintf(op_stat.str_dst, "r%d", op_stat.dst);
				break;
			case 1:
				sprintf(op_stat.str_dst, "(r%d)", op_stat.dst);
				break;
			case 2:
				sprintf(op_stat.str_dst, "(r%d)+", op_stat.dst);
				break;
			case 3:
				sprintf(op_stat.str_dst, "@(r%d)+", op_stat.dst);
				break;
			case 4:
				sprintf(op_stat.str_dst, "-(r%d)", op_stat.dst);
				break;
			case 5:
				sprintf(op_stat.str_dst, "@-(r%d)", op_stat.dst);
				break;
			case 6:
				sprintf(op_stat.str_dst, "%d(r%d)", (int16_t)read_word(greg.work+2), op_stat.dst);
				greg.work += 2;
				break;
			case 7:
				sprintf(op_stat.str_dst, "*%d(r%d)", (int16_t)read_word(greg.work+2), op_stat.dst);
				greg.work += 2;
				break;
		}
	}

}
char init_print_disas_ssdd(){
	init_str_src();
	init_str_dst();
}
char init_print_disas_dd()
{
	init_str_dst();
}
print_register(){
	if(dbg.flag == 0)return ;
	printf("%04x,%04x,%04x,%04x," 
			"%04x,%04x,sp=%04x,pc=%04x" 
			" | N:%d Z:%d V:%d C:%d : ",
			R[0],R[1],R[2],R[3],R[4],R[5],SP,PC-2,
			psw.N, psw.Z, psw.V, psw.C);
}
print_disas_ssdd(char *sys_call_str)
{
	if(dbg.flag == 0)return ;
	init_print_disas_ssdd();
	printf("%s %s, %s ", sys_call_str,
			op_stat.str_src,op_stat.str_dst);
}
print_disas_dd(char *sys_call_str)
{
	if(dbg.flag == 0)return ;
	init_print_disas_dd();
	printf("%s %s",sys_call_str,op_stat.str_dst);
}
print_disas_rss(char *sys_call_str)
{
	if(dbg.flag == 0)return ;
	init_print_disas_ssdd();
	printf("%s %s, r%d ", sys_call_str,
			op_stat.str_dst, op_stat.src);
}
print_disas(char *sys_call_str)
{
	if(dbg.flag == 0)return ;
	printf("%s", sys_call_str);
}
print_dst(char *n_str, uint16_t reg_dst){
	if(dbg.flag == 0)return ;
	printf("%s r%x",n_str, reg_dst);
}
print_src(char *n_str, uint16_t reg_src){
	if(dbg.flag == 0)return ;
	printf("%s r%x",n_str, reg_src);
}

print_disas_rdd(char *sys_call_str)
{
	if(dbg.flag == 0)return ;
	sprintf(op_stat.str_src, "r%d", op_stat.src);
	init_str_dst();
	printf("%s %s, %s ", sys_call_str,
			op_stat.str_src,op_stat.str_dst);
}
