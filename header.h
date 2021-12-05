#define MSIZE 0x40000
#define RSIZE 0x20
#define LSIZE 0x100
#define OSIZE 0x100000
#define ISIZE 0x10000
#include <math.h>
#include <stdio.h>


//0x10000000

union REG{
	int i;
	float f;
};

// global variable
unsigned int pc;
unsigned int count;
unsigned int jcnt;
unsigned int idx_in;
unsigned int idx_out;
unsigned int idx_ans;
unsigned int input_size;
unsigned int inst_size;
unsigned int inst_mem[OSIZE];
         int memory[MSIZE];
unsigned int labels[LSIZE];
union REG imm_mem[OSIZE];
FILE *log_file;

int s,t,d;
float fs,ft;

struct bf_r{
	unsigned int rt:6;
	unsigned int rs:6;
	unsigned int rd:6;
	unsigned int  j:1;
	unsigned int dm:1;
	unsigned int out:1;
	unsigned int in:1;
	unsigned int st:1;
	unsigned int ld:1;
	unsigned int  f:1;
	unsigned int op:4;
	unsigned int  b:1;
	unsigned int it:1;
	unsigned int is:1;
};

struct bf_b{
	unsigned int rt:6;
	unsigned int rs:6;
	unsigned int bimm:13;
	unsigned int op:4;
	unsigned int  b:1;
	unsigned int it:1;
	unsigned int is:1;
};

struct bf_j{
	unsigned int jimm1:6;
	unsigned int rs:6;
	unsigned int rd:6;
	unsigned int  j:1;
	unsigned int jimm2:10;
	unsigned int  b:1;
	unsigned int it:1;
	unsigned int is:1;
};

union INST{
	unsigned int inst;
	struct bf_r rinst;
	struct bf_b binst;
	struct bf_j jinst;
};

union REG registers[RSIZE];
union REG input_data[ISIZE];
char output_data[OSIZE];
char answer_data[OSIZE];

enum RNAME{
	ZERO,
	RA,
	SP,
	HP,
	CA,
	A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,
	S0,S1,S2,S3,S4,S5,S6,S7,
	D0,D1,D2
};

char regname[RSIZE][3];

// use in main.c
void read_cmd(char*);
void print_reg(void);
int finished;

// use in read.c
void read_program_file(void);
void read_imm_file(void);
void read_input_file(void);
void read_input_file_dummy(void);
void read_answer_file(void);
void read_global_data(void);

// use in init.c
void init(void);

// use in exec.c
void exec(void);
//void p_exec(void);
//void j_exec(void);
//void mem_exec(int,int);
int  check_sgn(unsigned int, int);

// use in fpu.c
float fadd(float, float);
float fsub(float, float);
float fmul(float, float);
float finv(float);
float fsqrt(float);
float itof(int);
int ftoi(float);
float ffloor(float);
float ffabs(float);
int fslt(float, float);
int fsle(float, float);
float fdiv(float, float);
float fexp2(float,int);

// use in print.c
void print_inst_i(char*,int,int,int);
void print_inst_m(char*,int,int,int);
void print_inst_f(char*,float,float,float);
void print_inst_j(char*,int,int,int);
void print_inst_b(char*,int,int,int);
void print_inst_fb(char*,float,float,int);
void print_inst_itof(char*,float,int,float);
void print_inst_ftoi(char*,int,float,float);
void print_stack();
void j_print_pc();
void r_print_i(int);
void r_print_f(int);
