#include <stdio.h>
#include "../header.h"

void print_inst_i(char* op, int d, int s, int t){
	fprintf(log_file,"%6s : %11d = %11d , %11d | %11d(pc), %11d(cc)\n", op, d, s, t, pc,count);
} //for add, sub, mul4, div2, seq, sne, slt, sle, fslt, fsle

void print_inst_m(char* op, int i, int m, int d){
	fprintf(log_file,"%6s : %11d m[%11d] =%11d | %11d(pc), %11d(cc)\n", op, d, i, m, pc,count);
} //for sw, lw


void print_inst_f(char* op, float d, float s, float t){
	fprintf(log_file,"%6s : %11f = %11f , %11f | %11d(pc), %11d(cc)\n", op, d, s, t, pc,count);
} //for fadd, fsub, fmul, fdiv, finv, fsqrt


void print_inst_itof(char* op, float d, int s, float dummy){
	fprintf(log_file,"%6s : %11f = %11d , %11f | %11d(pc), %11d(cc)\n", op, d, s, dummy, pc,count);
} //for itof

void print_inst_ftoi(char* op, int d, float s, float dummy){
	fprintf(log_file,"%6s : %11d = %11f , %11f | %11d(pc), %11d(cc)\n", op, d, s, dummy, pc,count);
} //for ftoi

void print_inst_b(char* op, int s, int t, int b){
	fprintf(log_file,"%6s : %11d - %11d , %11d | %11d(pc), %11d(cc)\n", op, b, s, t, pc,count);
} // beq, bne, blt, ble

void print_inst_fb(char* op, float s, float t, int b){
	fprintf(log_file,"%6s : %11d - %11f , %11f | %11d(pc), %11d(cc)\n", op, b, s, t, pc,count);
} // fbeq, fbne, fblt, fble

void print_inst_j(char* op, int s, int d, int n){
	fprintf(log_file,"%6s : %11d - %11d , %11d | %11d(pc), %11d(cc)\n", op, s, d, n, pc,count);
} // j

void print_stack(){
	int sp = registers[2].i;
	printf("stack:sp=%d pc=%d\n",sp,pc);
	for(int i=0; i<10; i++){
		printf("m[%11d] = %11d ",sp-i,memory[sp-i]);
		if (i%2==1) printf("\n");
	}
	printf("-------------------------------\n");
}

void j_print_pc(){
	fprintf(log_file,"%d\n",pc);
}

void r_print_i(int d){
	fprintf(log_file,"%d\n",registers[d].i);
}

void r_print_f(int d){
	fprintf(log_file,"%f\n",registers[d].f);
}
