#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header.h"

void read_program_file(void){
	FILE *fp;
	char *infile = "./src/program.bit";
	//char *infile = "./src/cos.bit";
	unsigned int index=0;
	char read[32];
	if ((fp = fopen(infile,"r"))==NULL){
		perror("read_program: failed to open input_file");
		exit(1);
	}
	while(fscanf(fp, "%s\n", read) != EOF){
		inst_mem[index] = (unsigned int)strtol(read,NULL,2);
		index++;
	}
	inst_size = index;
	printf("instruction_size = %d\n",inst_size);
	fclose(fp);
	return;
}

void read_imm_file(void){
	FILE *fp;
	char *infile = "./src/immediate.bit";
	//char *infile = "./src/cos_imm.bit";
	unsigned int index=0;
	char read[32];
	if ((fp = fopen(infile,"r"))==NULL){
		perror("read_program: failed to open input_file");
		exit(1);
	}
	while(fscanf(fp, "%s\n", read) != EOF){
		imm_mem[index].i = (int)strtol(read,NULL,2);
		index++;
	}
	if (inst_size != index) printf("immediate.bit size isnot equal to program.bit size\n");
	printf("instruction_size = %d\n",inst_size);
	fclose(fp);
	return;
}

void read_input_file(void){
	FILE *fp;
	char *infile = "./src/input.sld";
	char read[6];
	int f;
	if ((fp = fopen(infile,"r"))==NULL){
		perror("read_indata: failed to open input_file");
		exit(1);
	}
	while(fscanf(fp, "%s", read) != EOF){
		f=0;
		for(int i=0; i<6; i++){ if(read[i]=='.') f=1;} 
		if(f) input_data[idx_in].f = (float)atof(read);
		else  input_data[idx_in].i = (  int)atoi(read);
		idx_in ++;
		for(int i=0; i<6; i++) read[i] = 0;
	}
	fclose(fp);
	input_size = idx_in;
	printf("input-%dth=%d\n",input_size,input_data[input_size].i);
	//for(int i=0; i<input_size; i++){
		//printf("%10d ", input_data[i].i);
		//if (i%11 == 0) printf("\n");
	//}
	//printf("\n");
	idx_in = 0;
	return;
}

void read_answer_file(void){
	FILE *fp;
	char *answer = "./src/answer2.ppm";
	char read;
	if ((fp = fopen(answer,"r"))==NULL){ perror("failed to open answer_file"); exit(1);}
	while(fscanf(fp,"%c",&read) != EOF){
		answer_data[idx_ans] = read;
		idx_ans ++;
	}
	idx_ans = 0;
	fclose(fp);
}

void read_global_data(void){
	FILE *fp;
	char *infile = "./src/data.bit";
	unsigned int index=0;
	char read[32];
	if ((fp = fopen(infile,"r"))==NULL){
		perror("read_program: failed to open input_file");
		exit(1);
	}
	while(fscanf(fp, "%s\n", read) != EOF){
		//printf("%s ->",read);
		memory[index] = (int)strtol(read,NULL,2);
		//printf("%d\n",memory[index]);
		index++;
	}
	//registers[3].i = index;
	fclose(fp);
	return;
}

void read_input_file_dummy(void){
	input_data[0].f = 0.0;
	input_data[1].f = 0.0;
	input_data[2].f = 0.0;
	input_data[3].f = 0.0;
	input_data[4].f = 30.0;
	input_data[5].f = 1.0;
	input_data[6].f = 0.0;
	input_data[7].f = 0.0;
	input_data[8].f = 255.0;
	input_data[9].f = 255.0;
	input_data[10].f = 255.0;
	input_data[11].i = 0;
	input_data[12].i = 1;
	input_data[13].i = 2;
	input_data[14].i = 0;
	input_data[15].f = 40.0;
	input_data[16].f = 10.0;
	input_data[17].f = 40.0;
	input_data[18].f = 0.0;
	input_data[19].f = -40.0;
	input_data[20].f = 0.0;
	input_data[21].f = 1.0;
	input_data[22].f = 0.2;
	input_data[23].f = 64.0;
	input_data[24].f = 255.0;
	input_data[25].f = 255.0;
	input_data[26].f = 255.0;
	input_data[27].f = 0.0;
	input_data[28].i = 4;
	input_data[29].i = 3;
	input_data[30].i = 1;
	input_data[31].i = 0;
	input_data[32].f = 30.0;
	input_data[33].f = 30.0;
	input_data[34].f = 0.0;
	input_data[35].f = 0.0;
	input_data[36].f = 0.0;
	input_data[37].f = 1.0;
	input_data[38].f = 1.0;
	input_data[39].f = 255.0;
	input_data[40].f = 255.0;
	input_data[41].f = 255.0;
	input_data[42].f = 255.0;
	input_data[43].i = -1;
	input_data[44].i = 0;
	input_data[45].i = -1;
	input_data[46].i = 1;
	input_data[47].i = -1;
	input_data[48].i = -1;
	input_data[49].i = 99;
	input_data[50].i = 0;
	input_data[51].i = 1;
	input_data[52].i = -1;
	input_data[53].i = -1;
	input_size = 53;
	idx_in = 0;
	return;
}

