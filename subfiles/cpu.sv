
module core #(
  parameter RAM_SIZE_WIDTH = 18,
  parameter ROM_SIZE_WIDTH = 13,
  parameter CLK_PER_HALF_BIT = 434,
  parameter IBUF_SIZE_WIDTH = 4,
  parameter OBUF_SIZE_WIDTH = 6)
(
  input wire clk,
  input wire rst,
  input wire rxd,
  output wire txd,
  output wire [7:0] leds
);

typedef logic[5:0] regsize;
typedef logic[31:0] i32;

i32 operand1, operand2, alu_out, fpu_out;
logic [3:0] operator;
regsize RS_addr, RT_addr, RD_addr, RS_addr_EX, RT_addr_EX, RD_addr_EX;
i32 RS_value, RT_value, RS_value_EX, RT_value_EX, RS_forwarding_value, RT_forwarding_value, RD_value, RD_value_WB;
logic reg_write_enable;
i32 ram_read_value, ram_write_value;
logic rom_enable;
i32 pc, pc_reg;
i32 instr, immediate, immediate_EX;
i32 ram_addr;

logic [15:0] jump_immediate;
i32 jump_value, jump_forwarding_value;
logic [12:0] branch_immediate;
i32 branch_value, branch_value_EX;

logic IS, IT, IS_EX, IT_EX;
logic LD, ST, LD_EX, LD_WB, ST_EX;
logic J, J_EX, B, B_EX;
logic OUT, OUT_EX, OUT_WB, IN, IN_EX, IN_WB;
logic F, F_EX, F_WB;

logic uart_send_enable, uart_recv_enable, fpu_start, fpu_ready;

logic [1:0] stall;
logic flush;
logic branch_result;

alu alu(operand1, operand2, operator, alu_out);
fpu fpu(clk, rst, fpu_start, operator, operand1, operand2, fpu_ready, fpu_out);
register_file #(RAM_SIZE_WIDTH, 6) register_file(clk, rst, RS_addr, RT_addr, reg_write_enable, RD_addr, RD_value, RS_value, RT_value);
rom #(ROM_SIZE_WIDTH, "program.bit") instr_mem(clk, rom_enable, pc, instr);
rom #(ROM_SIZE_WIDTH, "immediate.bit") immediate_mem(clk, rom_enable, pc, immediate);
ram #(RAM_SIZE_WIDTH) data_mem (clk, ram_addr, ~stall[1], ST_EX, ram_write_value, ram_read_value);

logic uart_send_ready, uart_recv_ready;
logic [7:0] uart_send_data;
i32 uart_recv_data;
uart_sender #(CLK_PER_HALF_BIT, OBUF_SIZE_WIDTH) sender(clk, rst, uart_send_data, uart_send_enable, uart_send_ready, txd);
uart_receiver #(CLK_PER_HALF_BIT, IBUF_SIZE_WIDTH) receiver(clk, rst, rxd, uart_recv_enable, uart_recv_data, uart_recv_ready);


assign leds = pc[7:0];
assign jump_immediate = {instr[28:19], instr[5:0]};
assign branch_immediate = instr[24:12];
assign branch_value = {{19{branch_immediate[12]}}, branch_immediate};
assign RS_addr = instr[11:6];
assign RT_addr = instr[5:0];
assign uart_send_data = operand1[7:0];
assign fpu_start = F_EX & (~stall[1]);
assign uart_send_enable = OUT_EX & (~stall[1]);
assign uart_recv_enable = IN_EX & (~stall[1]);
assign branch_result = B_EX & alu_out[0];

// Instruction Fetch
always @(*) begin
  rom_enable = ~stall[0];
  if(branch_result) begin
    pc = pc_reg + branch_value_EX - 1;
  end else if(J) begin
    pc = jump_value;
  end else begin
    pc = pc_reg + 1;
  end
end
always @(posedge clk) begin
  if(rst) begin
    pc_reg <= 0;
  end else begin
    if(~stall[0]) begin
      pc_reg <= pc;
    end
  end
end

if(clk){
	rom_enable = !stall[0];
	if(branch_result){
		pc = pc_reg + brach_value_ex -1;
	}
	else if(J){
		pc = jump_value;
	}
	else{
		pc = pc_reg + 1;
	}
}

if(clk){
	if(rst){
		pc_reg = 0;
	}
	else{
		if(!stall[0]){
			pc_reg = pc;
		}
	}
}

if(clk){
// Register Fetch
always @(*) begin
  IS = instr[31];
  IT = instr[30];
  B = instr[29];
  J = instr[18] & (~B);
  if(J | B) begin
    F = 0;
    LD = 0;
    ST = 0;
    IN = 0;
    OUT = 0;
  end else begin
    F = instr[24];
    LD = instr[23];
    ST = instr[22];
    IN = instr[21];
    OUT = instr[20];
  end
  // Forwarding jump value
  if(RS_addr == RD_addr) begin
    jump_forwarding_value = RD_value;
  end else begin
    jump_forwarding_value = RS_value;
  end
  jump_value = (IS ? {16'b0, jump_immediate}: jump_forwarding_value);
end
if(clk){
	if(cur_inst.rinst.j | cur_inst.rinst.b){
		f = 0; ld =0; st=0; in=0; out=0;
	}
	else{
		f = instr[24]: ld=....;
	}
	if(RS_addr == RD_addr){
		jump_forwarding_value = RD_value;
	}
	else{
		jump_forwarding_value = RS_value;
	}
	jump_value = (cur_inst.rinst.is)*(jump_immdediate&&0xffff)+(!cur_inst.rinst.is)*(jump_forwading_value);
}
always @(posedge clk) begin
  if(rst | flush) begin
    branch_value_EX <= 0;
    RD_addr_EX <= 0;
    IS_EX <= 1;
    IT_EX <= 1;
    F_EX <= 0;
    B_EX <= 0;
    LD_EX <= 0;
    ST_EX <= 0;
    J_EX <= 0;
    IN_EX <= 0;
    OUT_EX <= 0;
    operator <= 0;
    RS_addr_EX <= 0;
    RT_addr_EX <= 0;
    RS_value_EX <= 0;
    RT_value_EX <= 0;
    immediate_EX <= 0;
  end else if(~stall[1]) begin
    operator <= instr[28:25];
    RS_addr_EX <= RS_addr;
    RT_addr_EX <= RT_addr;
    RS_value_EX <= RS_value;
    RT_value_EX <= RT_value;
    immediate_EX <= immediate;

    if(B | ST | OUT) begin // レジスタに書き込まない
      RD_addr_EX <= 0;
    end else begin
      RD_addr_EX <= instr[17:12];
    end
    IS_EX <= IS;
    IT_EX <= IT;
    F_EX <= F;
    B_EX  <= B;
    LD_EX <= LD;
    ST_EX <= ST;
    J_EX <= J;
    IN_EX <= IN;
    OUT_EX <= OUT;
    branch_value_EX <= branch_value;
  end
end

// EXecute
always @(*) begin
  // Forwarding Unit
  if(RS_addr_EX == RD_addr) begin
    RS_forwarding_value = RD_value;
  end else begin
    RS_forwarding_value = RS_value_EX;
  end
  if(RT_addr_EX == RD_addr) begin
    RT_forwarding_value = RD_value;
  end else begin
    RT_forwarding_value = RT_value_EX;
  end

  operand1 = (IS_EX ? immediate_EX: RS_forwarding_value);
  operand2 = (IT_EX ? immediate_EX: RT_forwarding_value);
  ram_addr = operand1 + operand2;
  ram_write_value = RS_forwarding_value;

end
always @(posedge clk) begin
  if(rst) begin
    RD_value_WB <= 0;
    RD_addr <= 0;
    reg_write_enable <= 0;
    F_WB <= 0;
    LD_WB <= 0;
    IN_WB <= 0;
  end else if(~stall[1]) begin
    if(B_EX | ST_EX | OUT_EX) begin // レジスタに書き込まない
      RD_value_WB <= 0;
    end else if(J_EX) begin
      RD_value_WB <= immediate_EX;
    end else begin
      RD_value_WB <= alu_out;
    end
    RD_addr <= RD_addr_EX;
    reg_write_enable <= 1;
    F_WB <= F_EX;
    LD_WB <= LD_EX;
    IN_WB <= IN_EX;
    OUT_WB <= OUT_EX;
  end
end

// Write Back
always @(*) begin
  if(IN_WB) begin
    RD_value = uart_recv_data;
  end else if(F_WB) begin
    RD_value = fpu_out;
  end else if(LD_WB) begin
    RD_value = ram_read_value;
  end else begin
    RD_value = RD_value_WB;
  end
end
always @(posedge clk) begin
end

// Hazard Unit
always @(*) begin
  if((OUT_WB && (~uart_send_ready)) || (IN_WB && (~uart_recv_ready))
  || (F_WB && (~fpu_ready))) begin
    stall = 2'b11;
    flush = 0;
  end else if(branch_result) begin
    stall = 2'b00;
    flush = 1;
  end else if(J && RS_addr != 0 && RS_addr == RD_addr_EX) begin
    stall = 2'b01;
    flush = 1;
  end else begin
    stall = 0;
    flush = 0;
  end
end

endmodule

`default_nettype wire
