#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "header.h"

int main(){

	init();
	read_program_file();
	read_imm_file();
	read_input_file();
	read_answer_file();
	read_global_data();

	char cmd[10];
	finished = 0;
	while(1){
		printf("(sim)");
		if (scanf("%s",cmd) == 0) exit(1);
		read_cmd(cmd);
		if (finished) break;
	}
	print_reg();
	fclose(log_file);
	FILE* output;
	output = fopen("./src/output.ppm","w");
	if (output == NULL) perror("oepn error");
	for(int i=0; i<idx_out; i++) fprintf(output, "%c",output_data[i]);
	fclose(output);
	return 0;
}


void read_cmd(char* c){
	if (strcmp(c,"run") == 0){
		struct timeval s,g;
		gettimeofday(&s,NULL);
		exec();
		gettimeofday(&g,NULL);
		finished = 1;
	  double t = (g.tv_sec-s.tv_sec)*1000000 + (g.tv_usec-s.tv_usec);
		printf("time: %fms, instruction: %d, ms/inst = %lf\n", t, count, (double)(t/count));
	}
	/*
	if (strcmp(c,"prun") == 0){
		struct timeval s,g;
		gettimeofday(&s,NULL);
		while (pc != -1){
			p_exec();
			count++;
		}
		finished = 1;
		gettimeofday(&g,NULL);
		for (int i=0; i<idx_out; i++){
			printf("%d ", output_data[i]);
			if (i%10 ==0) printf("\n");
		}
	  double t = (g.tv_sec-s.tv_sec)*0.000001 + (g.tv_usec-s.tv_usec);
		printf("time: %f, instruction: %d, ms/inst = %lf\n", t, count, (double)(t/count));
	}
	if (strcmp(c,"jrun") == 0){
		while (pc != -1){
			j_exec();
			count++;
			if(jcnt > 59942){
				printf("pc: %d\n, cc: %d, jc: %d\n",pc,count, jcnt);
			 	while(1);
			}
		}
		finished = 1;
	}
	else if (strcmp(c,"n")==0){
		printf("how steps?\n");
		int offset;
		if (scanf("%d",&offset) == 0) exit(1);
		while (offset>0){
			exec();
			offset--;
			count ++;
		}
	}
	else if (strcmp(c,"pn")==0){
		printf("how steps?\n");
		int offset;
		if (scanf("%d",&offset) == 0) exit(1);
		while (offset>0){
			p_exec();
			offset--;
			count ++;
		}
	}
	else if (strcmp(c,"b") ==0){
		int target_pc;
		printf("to where?\n");
		if (scanf("%d",&target_pc) == 0) exit(1);
		while (pc != -1 && pc != target_pc){
			exec();
			count ++;
		}
	}
	else if (strcmp(c,"pb") ==0){
		int target_pc;
		printf("to where?\n");
		if (scanf("%d",&target_pc) == 0) exit(1);
		while (pc != -1 && pc != target_pc){
			p_exec();
			count ++;
		}
	}
	else if (strcmp(c,"mem")==0){
		int target;
		printf("whats target?\n");
		if (scanf("%d",&target) == 0) exit(1);
		while (pc != -1 ){
			mem_exec(target,target);
			if(pc>302770) break;
			count++;
		}
	}
	else if (strcmp(c,"mems")==0){
		int target, target2;
		printf("whats targets?\n");
		if (scanf("%d %d",&target,&target2) == 0) exit(1);
		while (pc != -1){
			mem_exec(target,target2);
			count++;
		}
	}
	else if (strcmp(c,"runr")==0){
		int target_reg, val;
		printf("whats register?\n");
		if (scanf("%d",&target_reg) == 0) exit(1);
		val = registers[target_reg].i;
		while (pc != -1){
			exec();
			if (registers[target_reg].i != val){
				printf("pc: %10d, cc: %10d, %d -> %d\n",pc,count, val, registers[target_reg].i);
				val = registers[target_reg].i;
			}
			count++;
		}
	}
	else if (strcmp(c,"print")==0){
		char type;
		int  index, index2;
		printf("whats?\n");
		if (scanf("%c",&type) == 0) exit(1);
		if (type=='m'){
			if (scanf("%d",&index)==0) exit(1);
			printf("mem[%d] = %d\n", index, memory[index]);
		}
		else if (type=='r'){
			print_reg();
		}
		else if (type=='s'){
			print_stack();
		}
		else if (type=='n'){
			if (scanf("%d %d",&index, &index2)==0) exit(1);
			for(int i=0; i<index2; i++){
				printf("mem[%d] = %d\n", index+i, memory[index+i]);
			}
		}
	}*/
	else if (strcmp(c,"reg")==0){
		printf("pc: %10d, cc: %10d\n",pc,count);
		print_reg();
	}
	else if (strcmp(c,"quit") == 0){
		printf("quit\n");
		exit(0);
	}
	else{
		printf("---\n");
	}
	return;
}


void print_reg(){
	printf("ZERO = %10d %10f\n",registers[0].i,registers[0].f);
	printf("RA   = %10d %10f\n",registers[1].i,registers[1].f);
	printf("SP   = %10d %10f\n",registers[2].i,registers[2].f);
	printf("HP   = %10d %10f\n",registers[3].i,registers[3].f);
	printf("CA   = %10d %10f\n",registers[4].i,registers[4].f);
	printf("A0   = %10d %10f\n",registers[5].i,registers[5].f);
	printf("A1   = %10d %10f\n",registers[6].i,registers[6].f);
	printf("A2   = %10d %10f\n",registers[7].i,registers[7].f);
	printf("A3   = %10d %10f\n",registers[8].i,registers[8].f);
	printf("A4   = %10d %10f\n",registers[9].i,registers[9].f);
	printf("A5   = %10d %10f\n",registers[10].i,registers[10].f);
	printf("A6   = %10d %10f\n",registers[11].i,registers[11].f);
	printf("A7   = %10d %10f\n",registers[12].i,registers[12].f);
	printf("A8   = %10d %10f\n",registers[13].i,registers[13].f);
	printf("A9   = %10d %10f\n",registers[14].i,registers[14].f);
	printf("A10  = %10d %10f\n",registers[15].i,registers[15].f);
	printf("A11  = %10d %10f\n",registers[16].i,registers[16].f);
	printf("A12  = %10d %10f\n",registers[17].i,registers[17].f);
	printf("A13  = %10d %10f\n",registers[18].i,registers[18].f);
	printf("A14  = %10d %10f\n",registers[19].i,registers[19].f);
	printf("A15  = %10d %10f\n",registers[20].i,registers[20].f);
	printf("S0   = %10d %10f\n",registers[21].i,registers[21].f);
	printf("S1   = %10d %10f\n",registers[22].i,registers[22].f);
	printf("S2   = %10d %10f\n",registers[23].i,registers[23].f);
	printf("S3   = %10d %10f\n",registers[24].i,registers[24].f);
	printf("S4   = %10d %10f\n",registers[25].i,registers[25].f);
	printf("S5   = %10d %10f\n",registers[26].i,registers[26].f);
	printf("S6   = %10d %10f\n",registers[27].i,registers[27].f);
	printf("S7   = %10d %10f\n",registers[28].i,registers[28].f);
	return;
}
