#include <stdio.h>
#include <stdlib.h>
#include "../header.h"

int check_sgn(unsigned int n, int size){
	unsigned int msb = (1<<(size-1));
	if (n&msb) n-= (msb<<1);
	return n;
}

int is_immi(int b, int rs){ 
	return (b==1 ? (imm_mem+pc)->i : (registers+rs)->i);
}

float is_immf(int b, int rs){
	if (b) return (imm_mem+pc)->f;
	else   return (registers+rs)->f;
}

void exec(){
	while(pc!=-1){	
		union INST cur_inst;
		cur_inst.inst  = *(inst_mem+pc);
		if(cur_inst.rinst.b){
			d = cur_inst.binst.bimm;
			if (d>0x1000) d -= 0x2000;
			if (cur_inst.binst.op<12){
				s = is_immi(cur_inst.rinst.is, cur_inst.rinst.rs);
				t = is_immi(cur_inst.rinst.it, cur_inst.rinst.rt);
				switch(cur_inst.binst.op){
					case(0b1000): if (s==t) pc+= d; else pc++; break; //beq
					case(0b1001): if (s!=t) pc+= d; else pc++; break; //bne
					case(0b1010): if (s< t) pc+= d; else pc++; break;//blt
					case(0b1011): if (s<=t) pc+= d; else pc++; break;//ble
				}
			}
			else{
				fs = is_immf(cur_inst.rinst.is,cur_inst.rinst.rs);
				ft = is_immf(cur_inst.rinst.it,cur_inst.rinst.rt);
				switch(cur_inst.binst.op){
					case(0b1100):	if (fs==ft) pc+= d; else pc++; break; //fbeq
					case(0b1101): if (fs!=ft) pc+= d; else pc++; break;//fbne
					case(0b1110): if (fs<ft) pc+= d; else pc++; break; //fblt
					case(0b1111): if (fs<=ft) pc+= d; else pc++; break; //fble
					default: return;
				}
			}
		}
		else if(!cur_inst.rinst.j){
			d = cur_inst.rinst.rd;
			if(!cur_inst.rinst.f){
				s = is_immi(cur_inst.rinst.is, cur_inst.rinst.rs);
				t = is_immi(cur_inst.rinst.it, cur_inst.rinst.rt);
				if(cur_inst.rinst.it && cur_inst.rinst.is) (registers)->i = 0; 
				else if(cur_inst.rinst.ld){
					(registers+d)->i = *(memory+s+t); //lw
				}
				else if(cur_inst.rinst.st){
					s = registers[cur_inst.rinst.rs].i;
					t = registers[cur_inst.rinst.rt].i;
					d  = imm_mem[pc].i;
					*(memory+d+t) = s; //sw
				}
				else if(cur_inst.rinst.in){
					(registers+d)->i = (input_data+idx_in)->i; //in
					idx_in++;
				}
				else if(cur_inst.rinst.out){
					*(output_data+idx_out) = (unsigned char)(s & 0xff); //out
					idx_out++;
				}
				else{
					if(cur_inst.rinst.op<12){
						switch(cur_inst.rinst.op){
							case 0b0000: (registers+d)->i = s+t;       break; //add
							case 0b0001: (registers+d)->i = s-t;       break; //sub
							case 0b0010: (registers+d)->i = s*4;       break; //mul4
							case 0b0011: (registers+d)->i = s/2;       break; //div2
							case 0b1000: (registers+d)->i = (int)(s==t); break; //seq
							case 0b1001: (registers+d)->i = (int)(s!=t); break; //sne
							case 0b1010: (registers+d)->i = (int)(s< t); break; //slt
							case 0b1011: (registers+d)->i = (int)(s<=t); break; //sle
							default:
								printf("Invalid op %d pc:%d cc:%d\n",cur_inst.rinst.op,pc,count);
								exit(1);
						}
					}
					else{
						fs = is_immf(cur_inst.rinst.is,cur_inst.rinst.rs); 
						ft = is_immf(cur_inst.rinst.it,cur_inst.rinst.rt); 
						switch(cur_inst.rinst.op){
							case 0b1100: (registers+d)->i = (int)(s==t); break; //fseq
							case 0b1101: (registers+d)->i = (int)(fs!=ft); break; //fsne
							case 0b1110: (registers+d)->i = (int)(fs< ft); break; //fslt
							case 0b1111: (registers+d)->i = (int)(fs<=ft); break; //fsle
							default:
								printf("Invalid op %d pc:%d cc:%d\n",cur_inst.rinst.op,pc,count);
								exit(1);
						}
					}
				}
			}
			else{
				fs = is_immf(cur_inst.rinst.is,cur_inst.rinst.rs); 
				ft = is_immf(cur_inst.rinst.it,cur_inst.rinst.rt); 
				switch(cur_inst.rinst.op){
					case 0b0000: (registers+d)->f = fadd (fs,ft); break; //fadd
					case 0b0001: (registers+d)->f = fsub (fs,ft); break; //fsub
					case 0b0010: (registers+d)->f = fmul (fs,ft); break; //fmul
					case 0b0011: (registers+d)->f = fdiv (fs,ft); break; //fdiv
					case 0b0100: (registers+d)->f = finv (fs);   break; //finv
					case 0b0101: (registers+d)->f = fsqrt(fs);   break; //fsqrt
					case 0b1000: 
											 s = is_immi(cur_inst.rinst.is, cur_inst.rinst.rs);
											 (registers +d)->f = itof (s);   break; //itof
					case 0b1001: (registers+d)->i = ftoi (fs);   break; //ftoi
					case 0b1010: (registers+d)->f = ffloor(fs);   break; //floor
					case 0b1011: (registers+d)->f = ffabs (fs);   break; //ffabs
					case 0b1100: 
											 t = is_immi(cur_inst.rinst.it, cur_inst.rinst.rt);
											 (registers+d)->f = fexp2(fs,t);   break; //fexp2
					default:
						printf("Invalid op %d pc:%d cc:%d\n",cur_inst.rinst.op,pc,count);
						exit(1);
				}
			}
			pc++;
		}
		else{
			if (cur_inst.jinst.is) s = cur_inst.jinst.jimm1 + (cur_inst.jinst.jimm2<<6);
			else                   s = (registers+cur_inst.jinst.rs)->i;
			d = cur_inst.jinst.rd;
			t = imm_mem[pc].i;
			(registers+d)->i  = t;
			pc = s;
		}
		count++;
	}
	return ;
}
