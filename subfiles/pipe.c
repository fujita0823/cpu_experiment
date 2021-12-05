
int mux-pc(pc,aluout,IorD) return (IorD)*aluout+(!IorD)*pc;
int I/D-mem(A,WD,WE,clk) return RD;
int Sign-Extend(instr) return SignImm;
int reg-file(A1,A2,A3,WD3,WE3,clk) return [RD1,RD2];
int ALU(srcA,srcB,ALUControl) return ALUResult;
int reg-latch(in,clk,signal,reg) return out*clk*signal;


void cpu(){
	while(1){
		addr = mux-pc(pc,aluout,IorD);
		pre-instr = i/d-mem(addr,pre-srcb,memwrite,clk);
		reg-latch(pre-instr,clk,IRWrite,reg1);
		reg-latch(pre-instr,clk,0,reg2);
		reg-latch(reg1,clk,IRWrite,instr);
		reg-latch(reg2,clk,IRWrite,data);
		mux-dst(instr[16:20],instr[11:15],write3);
		reg-file(instr[21:25],instr[16:20],instra3,write3,RegWrite,clk,read1,read2);
		sign-extend(instr[0:15],signimm);
		reg-latch(read1,clk,0,reg3);
		reg-latch(read2,clk,0,reg4);
		reg-latch(reg3,clk,0,pre-srca);
		reg-latch(reg4,clk,0,pre-srcb);
		mux-srca(pc,pre-srca,srca,ALUsrcA);
		mux-srcb(pre-srcb,4,signimm,....,srcB,ALUsrcB1);
		ALU(srca,srcb,ALUresult,ALUconrlB2);
		reg-latch(ALUresult,clk,0,reg5);
		reg-latch(ALUresult,clk,PCWrite,reg6);
		reg-latch(reg5,clk,0,aluout);
		reg-latch(reg6,clk,PCWrite,pc);
		

	}
	return;
}
