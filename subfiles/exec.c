#include <stdio.h>
#include <stdlib.h>
#include "../header.h"

int check_sgn(unsigned int n, int size){
	unsigned int msb = (1<<(size-1));
	if (n&msb) n-= (msb<<1);
	return n;
}

int is_immi(int b, int rs){
	b ? return (imm_mem+pc)->i : return (registers+rs)->i;
}

float is_immf(int b, int rs){
	if (b) return (imm_mem+pc)->f;
	else   return (registers+rs)->f;
}

void exec(){
	union INST cur_inst;
	cur_inst.inst  = *(inst_mem+pc);
	if(cur_inst.rinst.b){
		unsigned int msb = 1<<12;
		d = cur_inst.binst.bimm
		if (d>msb) d -= (msb<<1);
		s = is_immi(cur_inst.rinst.is, cur_inst.rinst.rs);
		t = is_immi(cur_inst.rinst.it, cur_inst.rinst.rt);
		switch(cur_inst.binst.op){
			case(0b1000): if (s==t) pc+= d; else pc++; break; //beq
			case(0b1001): if (s!=t) pc+= d; else pc++; break; //bne
			case(0b1010): if (s< t) pc+= d; else pc++; break;//blt
			case(0b1011): if (s<=t) pc+= d; else pc++; break;//ble
			case(0b1100):	
											fs = is_immf(cur_inst.rinst.is,cur_inst.rinst.rs); 
											ft = is_immf(cur_inst.rinst.it,cur_inst.rinst.rt); 
											if (fs==ft) pc+= d; else pc++; break; //fbeq
			case(0b1101): 
											fs = is_immf(cur_inst.rinst.is,cur_inst.rinst.rs); 
											ft = is_immf(cur_inst.rinst.it,cur_inst.rinst.rt); 
											if (fs!=ft) pc+= d; else pc++; break;//fbne
			case(0b1110): 
											fs = is_immf(cur_inst.rinst.is,cur_inst.rinst.rs); 
											ft = is_immf(cur_inst.rinst.it,cur_inst.rinst.rt); 
											if (fs<ft) pc+= d; else pc++; break; //fblt
			case(0b1111): 
											fs = is_immf(cur_inst.rinst.is,cur_inst.rinst.rs); 
											ft = is_immf(cur_inst.rinst.it,cur_inst.rinst.rt); 
											if (fs<=ft) pc+= d; else pc++; break; //fble
			default: return;
		}
	}
	else if(cur_inst.rinst.j){
		if (cur_inst.jinst.is) s = cur_inst.jinst.jimm1 + (cur_inst.jinst.jimm2<<6);
		else                   s = (registers+cur_inst.jinst.rs)->i;
		d = cur_inst.jinst.rd;
		t = imm_mem[pc].i;
		(registers+d)->i  = t;
		pc = s;
	}
	else{
		d = cur_inst.rinst.rd;
		if(cur_inst.rinst.f){
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
		else{
			s = is_immi(cur_inst.rinst.is, cur_inst.rinst.rs);
			t = is_immi(cur_inst.rinst.it, cur_inst.rinst.rt);
			if(cur_inst.rinst.it && cur_inst.rinst.is) (registers)->i = 0; 
			else if(cur_inst.rinst.ld){
				(registers+d)->i = memory[s+t]; //lw
			}
			else if(cur_inst.rinst.st){
				s = registers[cur_inst.rinst.rs].i;
				t = registers[cur_inst.rinst.rt].i;
				d  = imm_mem[pc].i;
				*(memory+d+t) = s; //sw
			}
			else if(cur_inst.rinst.in){
				(registers+d)->i = input_data[idx_in].i; //in
				idx_in++;
			}
			else if(cur_inst.rinst.out){
				output_data[idx_out] = (unsigned char)(s & 0xff); //out
				idx_out++;
				idx_ans++;
			}
			else{
				switch(cur_inst.rinst.op){
					case 0b0000: (registers+d)->i = s+t;       break; //add
					case 0b0001: (registers+d)->i = s-t;       break; //sub
					case 0b0010: (registers+d)->i = s*4;       break; //mul4
					case 0b0011: (registers+d)->i = s/2;       break; //div2
					case 0b1000: (registers+d)->i = (int)(s==t); break; //seq
					case 0b1001: (registers+d)->i = (int)(s!=t); break; //sne
					case 0b1010: (registers+d)->i = (int)(s< t); break; //slt
					case 0b1011: (registers+d)->i = (int)(s<=t); break; //sle
					case 0b1100: 
											 fs = is_immf(cur_inst.rinst.is,cur_inst.rinst.rs); 
											 ft = is_immf(cur_inst.rinst.it,cur_inst.rinst.rt); 
											 (registers+d)->i = (int)(s==t); break; //fseq
					case 0b1101:
											 fs = is_immf(cur_inst.rinst.is,cur_inst.rinst.rs); 
											 ft = is_immf(cur_inst.rinst.it,cur_inst.rinst.rt); 
											 (registers+d)->i = (int)(fs!=ft); break; //fsne
					case 0b1110: 
											 fs = is_immf(cur_inst.rinst.is,cur_inst.rinst.rs); 
											 ft = is_immf(cur_inst.rinst.it,cur_inst.rinst.rt); 
											 (registers+d)->i = (int)(fs< ft); break; //fslt
					case 0b1111: 
											 fs = is_immf(cur_inst.rinst.is,cur_inst.rinst.rs); 
											 ft = is_immf(cur_inst.rinst.it,cur_inst.rinst.rt); 
											 (registers+d)->i = (int)(fs<=ft); break; //fsle
					default:
						printf("Invalid op %d pc:%d cc:%d\n",cur_inst.rinst.op,pc,count);
						exit(1);
				}
			}
		}
		pc++;
	}
	return ;
}

void p_exec(){
	if ((pc != -1) && pc<=inst_size && registers[2].i > registers[3].i){
		union INST cur_inst;
		int s,t,d; float fs,ft;
		cur_inst.inst  = inst_mem[pc];
		if(cur_inst.rinst.b){
			int d = cur_inst.binst.bimm;
			d = check_sgn(d, 13);
			if(cur_inst.binst.op>11){
				if(cur_inst.rinst.is) fs = imm_mem[pc].f;
				else                  fs = registers[cur_inst.rinst.rs].f;
				if(cur_inst.rinst.it) ft = imm_mem[pc].f;
				else                  ft = registers[cur_inst.rinst.rt].f;
				switch(cur_inst.binst.op){
					case(0b1100): 
						print_inst_fb("fbeq",fs,ft,d);
						if (fs==ft) pc+= d; else pc++; break; //fbeq
					case(0b1101): 
						print_inst_fb("fbne",fs,ft,d);
						if (fs!=ft) pc+= d; else pc++; break; //fbne
					case(0b1110): 
						print_inst_fb("fblt",fs,ft,d);
						if (fs< ft) pc+= d; else pc++; break; //fblt
					case(0b1111): 
						print_inst_fb("fble",fs,ft,d);
						if (fs<=ft) pc+= d; else pc++; break; //fble
					default: return;
				}
			}
			else{
				if(cur_inst.rinst.is) s = imm_mem[pc].i;
				else                  s = registers[cur_inst.rinst.rs].i;
				if(cur_inst.rinst.it) t = imm_mem[pc].i;
				else                  t = registers[cur_inst.rinst.rt].i;
				switch(cur_inst.binst.op){
					case(0b1000): 
						print_inst_i("beq",s,t,d);
						if (s==t) pc+= d; else pc++; break; //beq
					case(0b1001): 
						print_inst_i("bne",s,t,d);
						if (s!=t) pc+= d; else pc++; break; //bne
					case(0b1010): 
						print_inst_i("blt",s,t,d);
						if (s< t) pc+= d; else pc++; break; //blt
					case(0b1011): 
						print_inst_i("ble",s,t,d);
						if (s<=t) pc+= d; else pc++; break; //ble
					default: return;
				}
			}
		}
		else if(cur_inst.rinst.j){
			unsigned int si = cur_inst.jinst.jimm1 + (cur_inst.jinst.jimm2<<6);
			if (cur_inst.jinst.is) s = si;
			else                   s = registers[cur_inst.jinst.rs].i;
			t = imm_mem[pc].i;
			d = cur_inst.jinst.rd;
			registers[d].i  = t; //j
			if (d == 1){
				printf("------------------------\n");
				printf("j %s %d %d\n", regname[d],t,s);
			}
			print_inst_j("jmp",s,registers[d].i,t);
			pc = s;
		}
		else{
			d = cur_inst.rinst.rd;
			if(cur_inst.rinst.f){
				if(cur_inst.rinst.is) fs = imm_mem[pc].f;
				else                  fs = registers[cur_inst.rinst.rs].f;
				if(cur_inst.rinst.it) ft = imm_mem[pc].f;
				else                  ft = registers[cur_inst.rinst.rt].f;
				switch(cur_inst.rinst.op){
					case 0b0000: 
						registers[d].f = fadd (fs,ft); //fadd
						print_inst_f("fadd",registers[d].f,fs,ft); break;
					case 0b0001: 
						registers[d].f = fsub (fs,ft); //fsub
						print_inst_f("fsub",registers[d].f,fs,ft); break;
					case 0b0010: 
						registers[d].f = fmul (fs,ft); //fmul
						print_inst_f("fmul",registers[d].f,fs,ft); break;
					case 0b0011: 
						registers[d].f = fdiv (fs,ft); //fdiv
						print_inst_f("fdiv",registers[d].f,fs,ft); break;
					case 0b0100: 
						registers[d].f = finv (fs); //finv
						print_inst_f("finv",registers[d].f,fs,ft); break;
					case 0b0101: 
						registers[d].f = fsqrt(fs); //fsqrt
						print_inst_f("fsqrt",registers[d].f,fs,ft); break;
					case 0b1000: 
						if (cur_inst.rinst.is) s = imm_mem[pc].i;
						else s = registers[cur_inst.rinst.rs].i;
						registers[d].f = itof(s); //itof
						print_inst_itof("itof",registers[d].f,s,ft); break;
					case 0b1001: 
						registers[d].i = ftoi (fs); //ftoi
						print_inst_ftoi("ftoi",registers[d].i,fs,ft); break;
					case 0b1010: 
						registers[d].f = ffloor(fs); //floor
						print_inst_f("ffloor",registers[d].f,fs,ft); break;
					case 0b1011: 
						registers[d].f = ffabs (fs); //ffabs
						print_inst_f("ffabs",registers[d].f,fs,ft); break;
					case 0b1100: 
						if (cur_inst.rinst.it) t = imm_mem[pc].i;
						else t = registers[cur_inst.rinst.rt].i;
						registers[d].f = fexp2(fs,t); //fexp2
						print_inst_f("fexp2",registers[d].f,fs,t); break;
					default:
						printf("Invalid op %d pc:%d cc:%d\n",cur_inst.rinst.op,pc,count);
						exit(1);
				}
			}
			else{
				if(cur_inst.rinst.is) s = imm_mem[pc].i;
				else                  s = registers[cur_inst.rinst.rs].i;
				if(cur_inst.rinst.it) t = imm_mem[pc].i;
				else                  t = registers[cur_inst.rinst.rt].i;
				//printf("%d-> s= %d %d-> t = %d \n",cur_inst.rinst.is,s,cur_inst.rinst.it,t);
				int check;
				if(cur_inst.rinst.it && cur_inst.rinst.is){
					registers[d].i = 0; 
					print_inst_i("nop",registers[d].i, 0, 0); //nop
				}
				else if(cur_inst.rinst.ld){
					if (s+t == 0){
						printf("lw: memory[%10d] -> %10d %10f, pc %10d <- target\n",s+t, registers[d].i, registers[d].f,pc); 
					  printf("%d %d\n",check,memory[s+t]);
					}
					registers[d].i = memory[s+t]; //lw
					print_inst_m("lw",s+t,memory[s+t],registers[d].i);
				}
				else if(cur_inst.rinst.st){
					int rs = registers[cur_inst.rinst.rs].i;
					int rt = registers[cur_inst.rinst.rt].i;
				  int n  = imm_mem[pc].i;
					//if (s+t == 0){
					if (pc == 4045){
						printf("sw: memory[%10d] <- %10d  pc %10d <- target\n",s+rt, rs,pc); 
						check = memory[s+t];
					}
					memory[n+rt] = rs ; //sw
					print_inst_m("sw",n+rt,memory[n+rt],rs);
				}
				else if(cur_inst.rinst.in){
					if (idx_in > input_size) {printf("out of range -- %d\n", idx_in); while(1);}
					printf("input ------- %10d\n",input_data[idx_in].i);
					registers[d].i = input_data[idx_in].i; //in
					idx_in++;
				}
				else if(cur_inst.rinst.out){
					output_data[idx_out] = (char)(s & 0xff); //out
					int ans = 0; if(idx_ans>0) ans = idx_ans-1;
					printf("output ------%10d-> %10d (%10d)\n", s,output_data[idx_out],answer_data[ans]);
					idx_out++;
					idx_ans++;
				}
				else{
					switch(cur_inst.rinst.op){
						case 0b0000: 
							registers[d].i = s+t;
							print_inst_i("add",registers[d].i,s,t); break; //add
						case 0b0001: 
							registers[d].i = s-t;       
							print_inst_i("sub",registers[d].i,s,t); break; //sub
						case 0b0010: 
							registers[d].i = s*4;       
							print_inst_i("mul4",registers[d].i,s,t); break; //mul4
						case 0b0011: 
							registers[d].i = s/2;       
							print_inst_i("div2",registers[d].i,s,t); break; //div2
						case 0b1000: 
							registers[d].i = (int)(s==t);
							print_inst_i("seq",registers[d].i,s,t); break; //seq
						case 0b1001: 
							registers[d].i = (int)(s!=t); 
							print_inst_i("sne",registers[d].i,s,t); break; //sne
						case 0b1010: 
							registers[d].i = (int)(s< t); 
							print_inst_i("slt",registers[d].i,s,t); break; //slt
						case 0b1011: 
							registers[d].i = (int)(s<=t);
							print_inst_i("sle",registers[d].i,s,t); break; //sle
						case 0b1100: 
												if(cur_inst.rinst.is) fs = imm_mem[pc].f;
												else                  fs = registers[cur_inst.rinst.rs].f;
												if(cur_inst.rinst.it) ft = imm_mem[pc].f;
												else                  ft = registers[cur_inst.rinst.rt].f;
												 registers[d].i = (int)(fs==ft); break; //fseq
							print_inst_i("fseq",registers[d].i,fs,ft); break; //fseq
						case 0b1101: 
												if(cur_inst.rinst.is) fs = imm_mem[pc].f;
												else                  fs = registers[cur_inst.rinst.rs].f;
												if(cur_inst.rinst.it) ft = imm_mem[pc].f;
												else                  ft = registers[cur_inst.rinst.rt].f;
												 registers[d].i = (int)(fs!=ft); break; //fsne
							print_inst_i("fsne",registers[d].i,fs,ft); break; //fsne
						case 0b1110: 
												if(cur_inst.rinst.is) fs = imm_mem[pc].f;
												else                  fs = registers[cur_inst.rinst.rs].f;
												if(cur_inst.rinst.it) ft = imm_mem[pc].f;
												else                  ft = registers[cur_inst.rinst.rt].f;
												 registers[d].i = (int)(fs< ft); break; //fslt
							print_inst_i("fslt",registers[d].i,fs,ft); break; //fslt
						case 0b1111: 
												if(cur_inst.rinst.is) fs = imm_mem[pc].f;
												else                  fs = registers[cur_inst.rinst.rs].f;
												if(cur_inst.rinst.it) ft = imm_mem[pc].f;
												else                  ft = registers[cur_inst.rinst.rt].f;
												 registers[d].i = (int)(fs<=ft); break; //fsle
							print_inst_i("fsle",registers[d].i,fs,ft); break; //fsle
						default:
							printf("Invalid op %d pc:%d cc:%d\n",cur_inst.rinst.op,pc,count);
							exit(1);
					}
				}
			}
			pc++;
		}
	}
	return ;
}

void mem_exec(int target,int target2){
	if ((pc != -1) && pc<=inst_size && registers[2].i > registers[3].i){
		union INST cur_inst;
		int s,t,d; float fs,ft;
		cur_inst.inst  = inst_mem[pc];
		if(cur_inst.rinst.b){
			d = cur_inst.binst.bimm;
			d = check_sgn(d, 13);
			if(cur_inst.rinst.is) s = imm_mem[pc].i;
			else                  s = registers[cur_inst.rinst.rs].i;
			if(cur_inst.rinst.it) t = imm_mem[pc].i;
			else                  t = registers[cur_inst.rinst.rt].i;
			switch(cur_inst.binst.op){
				case(0b1000): if (s==t) pc+= d; else pc++; break; //beq
				case(0b1001): if (s!=t) pc+= d; else pc++; break; //bne
				case(0b1010): if (s<t) pc+= d; else pc++; break;//blt
				case(0b1011): if (s<=t) pc+= d; else pc++; break;//ble
				case(0b1100): if (s==t) pc+= d; else pc++; break; //fbeq
				case(0b1101): if (s!=t) pc+= d; else pc++; break;//fbne
				case(0b1110): if (s< t) pc+= d; else pc++; break; //fblt
				case(0b1111): if (s<=t) pc+= d; else pc++; break; //fble
				default: return;
			}
		}
		else if(cur_inst.rinst.j){
			unsigned int si = cur_inst.jinst.jimm1 | (cur_inst.jinst.jimm2<<6);
			if (cur_inst.jinst.is) s = si;
			else{
				s = registers[cur_inst.jinst.rs].i;
				//if (cur_inst.jinst.rs==1) printf("pc: %10d, cc: %10d, ra=%10d\n",pc,count,s);
			}
			t = imm_mem[pc].i;
			d = cur_inst.jinst.rd;
			registers[d].i  = t; //j
			pc = s; //j
		}
		else{
			d = cur_inst.rinst.rd;
			if(cur_inst.rinst.f){
				if(cur_inst.rinst.is) fs = imm_mem[pc].f;
				else                  fs = registers[cur_inst.rinst.rs].f;
				if(cur_inst.rinst.it) ft = imm_mem[pc].f;
				else                  ft = registers[cur_inst.rinst.rt].f;
				switch(cur_inst.rinst.op){
					case 0b0000: registers[d].f = fadd (fs,ft); break; //fadd
					case 0b0001: registers[d].f = fsub (fs,ft); break; //fsub
					case 0b0010: registers[d].f = fmul (fs,ft); break; //fmul
					case 0b0011: registers[d].f = fdiv (fs,ft); break; //fdiv
					case 0b0100: registers[d].f = finv (fs);   break; //finv
					case 0b0101: registers[d].f = fsqrt(fs);   break; //fsqrt
					case 0b1000: if(cur_inst.rinst.is) s = imm_mem[pc].i; else s = registers[cur_inst.rinst.rs].i; 
											 registers[d].f = itof (s);   break; //itof
					case 0b1001: registers[d].i = ftoi (fs);  break; //ftoi
					case 0b1010: registers[d].f = ffloor(fs);   break; //floor
					case 0b1011: registers[d].f = ffabs (fs);   break; //ffabs
					case 0b1100: if (cur_inst.rinst.it) t = imm_mem[pc].i; else t = registers[cur_inst.rinst.rt].i;
											 registers[d].f = fexp2(fs,t);   break; //fexp2
					default:
						printf("Invalid op %d pc:%d cc:%d\n",cur_inst.rinst.op,pc,count);
						exit(1);
				}
			}
			else{
				if(cur_inst.rinst.is) s = imm_mem[pc].i;
				else                  s = registers[cur_inst.rinst.rs].i;
				if(cur_inst.rinst.it) t = imm_mem[pc].i;
				else                  t = registers[cur_inst.rinst.rt].i;
				if(cur_inst.rinst.it && cur_inst.rinst.is){
					registers[d].i = 0; 
				}
				else if(cur_inst.rinst.ld){
					registers[d].i = memory[s+t]; //lw
					//if ((s+t >= target) && (s+t <= target2)){
						//printf("lw: memory[%10d] -> %10d, pc %10d cc %10d\n",s+t, registers[d].i, pc, count); 
					//}
				}
				else if(cur_inst.rinst.st){
					s = registers[cur_inst.rinst.rs].i;
					t = registers[cur_inst.rinst.rt].i;
					d  = imm_mem[pc].i;
					memory[d+t] = s; //sw
					if ((d+t >= target) && (d+t <= target2)){
						printf("sw: memory[%10d] <- %10d, pc %10d cc %10d\n",d+t, s, pc,count); 
					}
				}
				else if(cur_inst.rinst.in){
					printf("%d\n",input_data[idx_in].i);
					registers[d].i = input_data[idx_in].i; //in
					idx_in++;
				}
				else if(cur_inst.rinst.out){
					int out = 0; if (idx_out>0) out = idx_out-1;
					output_data[out] = (unsigned char)(s & 0xff); //out
					if (output_data[out] != answer_data[idx_ans]){
					  printf("pc: %d, cc:%d, out: %d ans: %d\n",pc, count,output_data[out], answer_data[idx_ans]);
						//if (out>1) while(1);
					}else{
					  printf("pc: %d, cc:%d, out: %d ans: %d\n",pc, count,output_data[out], answer_data[idx_ans]);
					}
					idx_out++;
					idx_ans++;
				}
				else{
					switch(cur_inst.rinst.op){
						case 0b0000: registers[d].i = s+t;       break; //add
						case 0b0001: registers[d].i = s-t;       break; //sub
						case 0b0010: registers[d].i = s*4;       break; //mul4
						case 0b0011: registers[d].i = s/2;       break; //div2
						case 0b1000: registers[d].i = (int)(s==t); break; //seq
						case 0b1001: registers[d].i = (int)(s!=t); break; //sne
						case 0b1010: registers[d].i = (int)(s< t); break; //slt
						case 0b1011: registers[d].i = (int)(s<=t); break; //sle
						case 0b1100: registers[d].i = (int)(s==t); break; //fseq
						case 0b1101: registers[d].i = (int)(s!=t); break; //fsne
						case 0b1110: registers[d].i = (int)(s< t); break; //fslt
						case 0b1111: registers[d].i = (int)(s<=t); break; //fsle
						default:
							printf("Invalid op %d pc:%d cc:%d\n",cur_inst.rinst.op,pc,count);
							exit(1);
					}
				}
			}
			pc++;
		}
	}
	return ;
}

void j_exec(){
	if ((pc != -1) && pc<=inst_size && registers[2].i > registers[3].i){
		union INST cur_inst;
		int s,t,d; float fs,ft;
		cur_inst.inst  = inst_mem[pc];
		if(cur_inst.rinst.b){
			d = check_sgn(cur_inst.binst.bimm,13);
			if(cur_inst.rinst.is) s = imm_mem[pc].i;
			else                  s = registers[cur_inst.rinst.rs].i;
			if(cur_inst.rinst.it) t = imm_mem[pc].i;
			else                  t = registers[cur_inst.rinst.rt].i;
			switch(cur_inst.binst.op){
				case(0b1000): if (s==t) pc+= d; else pc++; break; //beq
				case(0b1001): if (s!=t) pc+= d; else pc++; break; //bne
				case(0b1010): if (s< t) pc+= d; else pc++; break;//blt
				case(0b1011): if (s<=t) pc+= d; else pc++; break;//ble
				case(0b1100): if (s==t) pc+= d; else pc++; break; //fbeq
				case(0b1101): if (s!=t) pc+= d; else pc++; break;//fbne
				case(0b1110): if (s<t) pc+= d; else pc++; break; //fblt
				case(0b1111): if (s<=t) pc+= d; else pc++; break; //fble
				default: return;
			}
			j_print_pc();
			jcnt ++;
		}
		else if(cur_inst.rinst.j){
			if (cur_inst.jinst.is) s = cur_inst.jinst.jimm1 + (cur_inst.jinst.jimm2<<6);
			else                   s = registers[cur_inst.jinst.rs].i;
			d = cur_inst.jinst.rd;
			t = imm_mem[pc].i;
			registers[d].i  = t;
			pc = s;
			j_print_pc();
			jcnt++;
		}
		else{
			d = cur_inst.rinst.rd;
			if(cur_inst.rinst.f){
				if(cur_inst.rinst.is) fs = imm_mem[pc].f;
				else                  fs = registers[cur_inst.rinst.rs].f;
				if(cur_inst.rinst.it) ft = imm_mem[pc].f;
				else                  ft = registers[cur_inst.rinst.rt].f;
				switch(cur_inst.rinst.op){
					case 0b0000: registers[d].f = fadd (fs,ft); break; //fadd
					case 0b0001: registers[d].f = fsub (fs,ft); break; //fsub
					case 0b0010: registers[d].f = fmul (fs,ft); break; //fmul
					case 0b0011: registers[d].f = fdiv (fs,ft); break; //fdiv
					case 0b0100: registers[d].f = finv (fs);   break; //finv
					case 0b0101: registers[d].f = fsqrt(fs);   break; //fsqrt
					case 0b1000: if(cur_inst.rinst.is) s = imm_mem[pc].i; else s = registers[cur_inst.rinst.rs].i; 
											 registers[d].f = itof (s);   break; //itof
					case 0b1001: registers[d].i = ftoi (fs);   break; //ftoi
					case 0b1010: registers[d].f = ffloor(fs);   break; //floor
					case 0b1011: registers[d].f = ffabs (fs);   break; //ffabs
					case 0b1100: if (cur_inst.rinst.it) t = imm_mem[pc].i; else t = registers[cur_inst.rinst.rt].i;
											 registers[d].f = fexp2(fs,t);   break; //fexp2
					default:
						printf("Invalid op %d pc:%d cc:%d\n",cur_inst.rinst.op,pc,count);
						exit(1);
				}
			}
			else{
				if(cur_inst.rinst.is) s = imm_mem[pc].i;
				else                  s = registers[cur_inst.rinst.rs].i;
				if(cur_inst.rinst.it) t = imm_mem[pc].i;
				else                  t = registers[cur_inst.rinst.rt].i;
				if(cur_inst.rinst.it && cur_inst.rinst.is) registers[d].i = 0; 
				else if(cur_inst.rinst.ld){
					registers[d].i = memory[s+t]; //lw
				}
				else if(cur_inst.rinst.st){
					s = registers[cur_inst.rinst.rs].i;
					t = registers[cur_inst.rinst.rt].i;
					d  = imm_mem[pc].i;
					memory[d+t] = s; //sw
				}
				else if(cur_inst.rinst.in){
					if(idx_in > input_size){ printf("out of range = %d\n",input_size); while(1);}
					registers[d].i = input_data[idx_in].i; //in
					idx_in++;
				}
				else if(cur_inst.rinst.out){
					int out = 0; if (idx_out>0) out = idx_out-1;
					output_data[out] = (unsigned char)(s & 0xff); //out
					if (output_data[out] != answer_data[idx_ans]){
						//if (out>0) while(1);
					}
					idx_out++;
					idx_ans++;
				}
				else{
					switch(cur_inst.rinst.op){
						case 0b0000: registers[d].i = s+t;       break; //add
						case 0b0001: registers[d].i = s-t;       break; //sub
						case 0b0010: registers[d].i = s*4;       break; //mul4
						case 0b0011: registers[d].i = s/2;       break; //div2
						case 0b1000: registers[d].i = (int)(s==t); break; //seq
						case 0b1001: registers[d].i = (int)(s!=t); break; //sne
						case 0b1010: registers[d].i = (int)(s< t); break; //slt
						case 0b1011: registers[d].i = (int)(s<=t); break; //sle
						case 0b1100: registers[d].i = (int)(s==t); break; //fseq
						case 0b1101: registers[d].i = (int)(s!=t); break; //fsne
						case 0b1110: registers[d].i = (int)(s< t); break; //fslt
						case 0b1111: registers[d].i = (int)(s<=t); break; //fsle
						default:
							printf("Invalid op %d pc:%d cc:%d\n",cur_inst.rinst.op,pc,count);
							exit(1);
					}
				}
			}
			pc++;
		}
	}
	return ;
}
