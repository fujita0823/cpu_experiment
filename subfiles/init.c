#include "../header.h"
#include <stdio.h>
#include <stdlib.h>

void init(){
	pc      = 0;
	count   = 0;
	jcnt    = 0;
	idx_in  = 0;
	idx_out = 0;
	//log_file= fopen("log.txt","w");
	//log_file= fopen(1,"w");
	log_file = stdout;
	if (log_file == NULL){
		perror("open error");
		exit(1);
	}
	for(int i=0; i<MSIZE; i++){
		memory[i] = 0;
	}
	printf("MSIZE-1=%d\n",MSIZE-1);
	for(int i=0; i<RSIZE; i++){
		if      (i==1) registers[i].i = -1;
		else if (i==2) registers[i].i = MSIZE;
		else           registers[i].i = 0;
	}
	for(int i=0; i<OSIZE; i++){
		output_data[i] = (char)0;
	}

	char reg_name[RSIZE][3] = {"zr","ra","sp","hp","ca",
		"A0","A1","A2","A3","A4","A5","A6","A7","A8","A9","A10","A11","A12","A13","A14","A15",
		"S0","S1","S2","S3","S4","S5","S6","S7",
		"D0","D1","D2"};
	for(int i=0; i<RSIZE; i++){
		for(int j=0; j<3; j++){
			regname[i][j] = reg_name[i][j];
		}
	}

	return;
}
