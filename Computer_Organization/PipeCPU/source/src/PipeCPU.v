//	Title: MIPS Single-Cycle Processor
//	Editor: Selene (Computer System and Architecture Lab, ICE, CYCU)
module PipeCPU( clk, rst );
	input clk, rst;
	
	// instruction bus
	wire[31:0] instr_IF, instr_ID;
	
	// break out important fields from instruction
	wire [5:0] opcode, opcode_IF, funct, funct_IF, alu_singal, sht_singal, mux_singal;
	wire [5:0] multu_singal;
    wire [4:0] rs, rt_ID, rt_EX, rd_ID, rd_EX, shamt;
    wire [15:0] immed;
    wire [31:0] extend_immed_ID, extend_immed_EX, b_offset, extend_shamt_ID, extend_shamt_EX;
    wire [25:0] jumpoffset_ID, jumpoffset_EX;
	
	// datapath signals
    wire [4:0] rfile_wn_EX, rfile_wn_MEM, rfile_wn_WB;
    wire [31:0] rfile_rd1_ID, rfile_rd1_EX, rfile_rd2_ID, rfile_rd2_EX, rfile_rd2_MEM,
				rfile_wd, alu_b, alu_out, sht_out, hi_out, lo_out, 
				talu_out_EX, talu_out_MEM, talu_out_WB, b_tgt, pc_next,
                pc, pc_incr_IF, pc_incr_ID, pc_incr_EX, dmem_rdata_MEM, dmem_rdata_WB, jump_addr, branch_addr;
	wire [63:0] multu_out;

	// control signals
	// wire RegRN1, RegWN, RegWrite, exSrc, ALUSrc, MemRead, MemWrite, MemToReg, PCSrc, Jump;
    wire Branch_EQ, Branch_EQ_EX, Branch_NE, Branch_NE_EX,
		 PCSrc, PCSrc1, PCSrc2 , RegDst, RegDst_EX,
		 MemRead, MemRead_EX, MemRead_MEM, MemWrite, MemWrite_EX, MemWrite_MEM,
		 ALUSrc, ALUSrc_EX, Zero, Jump, Jump_EX,
		 RegWrite, RegWrite_EX, RegWrite_MEM, RegWrite_WB,
		 MemtoReg, MemtoReg_EX, MemtoReg_MEM, MemtoReg_WB;
    wire [1:0] ALUOp_ID, ALUOp_EX;
    wire [2:0] Operation;
	
	
	// handle PC
	reg32 PC( .clk(clk), .rst(rst), .en_reg(1'b1), .d_in(pc_next), .d_out(pc) );
	add32 PCADD( .a(pc), .b(32'd4), .result(pc_incr_IF) );
	mux2 #(32) PCMUX( .sel(PCSrc), .a(pc_incr_IF), .b(b_tgt), .y(branch_addr) );
	mux2 #(32) JMUX( .sel(Jump_EX), .a(branch_addr), .b(jump_addr), .y(pc_next) );
	
	// handle Instruction Memory
	memory InstrMem( .clk(clk), .MemRead(1'b1), .MemWrite(1'b0), .wd(32'd0), .addr(pc), .rd(instr_IF) );
	assign opcode_IF = instr_IF[31:26];
	assign funct_IF = instr_IF[5:0];
	
	// Reg IF/ID ---------------------------------------------------------
	// pc_incr, instr
	reg_pipe IF_ID( .clk(clk), .rst(rst), .en_w(1'b1),
					.r32_in1(pc_incr_IF), .r32_out1(pc_incr_ID),
					.r32_in2(instr_IF), .r32_out2(instr_ID));
	
	// instruction fetch (done)
	assign opcode = instr_ID[31:26];
    assign rs = instr_ID[25:21];
    assign rt_ID = instr_ID[20:16];
    assign rd_ID = instr_ID[15:11];
    assign shamt = instr_ID[10:6];
    assign immed = instr_ID[15:0];
    assign jumpoffset_ID = instr_ID[25:0];
	
	// handle Control
	control_single CTL(.instr(instr_ID), .RegDst(RegDst), .RegWrite(RegWrite), .ALUSrc(ALUSrc),
					   .MemRead(MemRead), .MemWrite(MemWrite), .MemtoReg(MemtoReg), 
                       .Branch_EQ(Branch_EQ), .Branch_NE(Branch_NE), .Jump(Jump), .ALUOp(ALUOp_ID));
	
	// handle RegFile
	reg_file RegFile( .clk(clk), .RegWrite(RegWrite_WB), .RN1(rs), .RN2(rt_ID), .WN(rfile_wn_WB), 
					  .WD(rfile_wd), .RD1(rfile_rd1_ID), .RD2(rfile_rd2_ID) );
					  
	// handle EX imm(offset)/shamt
	sign_extend SignExt1( .immed_in(immed), .ext_immed_out(extend_immed_ID) );
	sign_extend SignExt2( .immed_in(shamt), .ext_immed_out(extend_shamt_ID) );
	
	// Reg ID/EX ---------------------------------------------------------
	// jumpoffset, rfile_rd1, rfile_rd2, extend_immed, extend_shamt, rd, rt
	reg_pipe ID_EX( .clk(clk), .rst(rst), .en_w(1'b1),
					.r26_in1(jumpoffset_ID), .r26_out1(jumpoffset_EX),
					.r32_in1(pc_incr_ID), .r32_out1(pc_incr_EX),
					.r32_in2(rfile_rd1_ID), .r32_out2(rfile_rd1_EX),
					.r32_in3(rfile_rd2_ID), .r32_out3(rfile_rd2_EX),
					.r32_in4(extend_immed_ID), .r32_out4(extend_immed_EX),
					.r32_in5(extend_shamt_ID), .r32_out5(extend_shamt_EX),
					.r5_in1(rd_ID), .r5_out1(rd_EX),
					.r5_in2(rt_ID), .r5_out2(rt_EX),
					.r2_in1(ALUOp_ID), .r2_out1(ALUOp_EX),
					.e_in1(RegDst), .e_out1(RegDst_EX),
					.e_in2(ALUSrc), .e_out2(ALUSrc_EX),
					.e_in3(Branch_EQ), .e_out3(Branch_EQ_EX),
					.e_in4(Branch_NE), .e_out4(Branch_NE_EX),
					.e_in5(Jump), .e_out5(Jump_EX),
					.m_in1(MemRead), .m_out1(MemRead_EX),
					.m_in2(MemWrite), .m_out2(MemWrite_EX),
					.w_in1(MemtoReg), .w_out1(MemtoReg_EX),
					.w_in2(RegWrite), .w_out2(RegWrite_EX));
	
	// handle Jump
	assign jump_addr = { pc_incr_EX[31:28], jumpoffset_EX << 2 };
	
	// handle Branch
	assign b_offset = extend_immed_EX << 2;
	add32 BRADD( .a(pc_incr_EX), .b(b_offset), .result(b_tgt) );
	and BR_AND1(PCSrc1, Branch_EQ_EX, Zero);
	and BR_AND2(PCSrc2, Branch_NE_EX, ~Zero);
	or BR_AND(PCSrc, PCSrc1, PCSrc2);
	
	// handle MUX WN
	mux2 #(5) RFMUX( .sel(RegDst_EX), .a(rd_EX), .b(rt_EX), .y(rfile_wn_EX) );
	
	// handle tb ALU
	assign funct = extend_immed_EX[5:0];
	aluControl ALUCTL( .clk(clk), .ALUOp(ALUOp_EX), .Signal(funct), .SignaltoALU(alu_singal), .SignaltoSHT(sht_singal), .SignaltoMULT(multu_singal), .SignaltoMUX(mux_singal) );
	mux2 #(32) ALUMUX( .sel(ALUSrc_EX), .a(rfile_rd2_EX), .b(extend_immed_EX), .y(alu_b) );
	alu ALU( .dataA(rfile_rd1_EX), .dataB(alu_b), .Signal(alu_singal), .dataOut(alu_out), .zero(Zero) );
	shifter SHIFTER( .dataA(rfile_rd2_EX), .dataB(extend_shamt_EX), .Signal(sht_singal), .dataOut(sht_out) );
	multiplier MULTIPLIER( .clk(clk), .dataA(rfile_rd1_EX), .dataB(rfile_rd2_EX), .Signal(multu_singal), .dataOut(multu_out) );
	hilo HILO( .clk(clk), .Ans(multu_out), .HiOut(hi_out), .LoOut(lo_out) );
	mux MUX( .ALUOut(alu_out), .HiOut(hi_out), .LoOut(lo_out), .Shifter(sht_out), .Signal(mux_singal), .dataOut(talu_out_EX) );
	
    // Reg EX/MEM --------------------------------------------------------
	// talu_out, rfile_rd2, rfile_wn
    reg_pipe EX_MEM ( .clk(clk), .rst(rst), .en_w(1'b1),
					  .r32_in1(talu_out_EX), .r32_out1(talu_out_MEM),
					  .r32_in2(rfile_rd2_EX), .r32_out2(rfile_rd2_MEM),
					  .r5_in1(rfile_wn_EX), .r5_out1(rfile_wn_MEM),
					  .m_in1(MemRead_EX), .m_out1(MemRead_MEM),
					  .m_in2(MemWrite_EX), .m_out2(MemWrite_MEM),
					  .w_in1(MemtoReg_EX), .w_out1(MemtoReg_MEM),
					  .w_in2(RegWrite_EX), .w_out2(RegWrite_MEM));
	
	// handle Data Memory
	memory DatMem( .clk(clk), .MemRead(MemRead_MEM), .MemWrite(MemWrite_MEM), .wd(rfile_rd2_MEM), 
				   .addr(talu_out_MEM), .rd(dmem_rdata_MEM) );

	// Reg MEM/WB --------------------------------------------------------
	// talu_out, dmem_rdata, rfile_wn
	reg_pipe MEM_WB ( .clk(clk), .rst(rst), .en_w(1'b1),
					  .r32_in1(talu_out_MEM), .r32_out1(talu_out_WB),
					  .r32_in2(dmem_rdata_MEM), .r32_out2(dmem_rdata_WB),
					  .r5_in1(rfile_wn_MEM), .r5_out1(rfile_wn_WB),
					  .w_in1(MemtoReg_MEM), .w_out1(MemtoReg_WB),
					  .w_in2(RegWrite_MEM), .w_out2(RegWrite_WB));
	
	// handle MUX MemToReg
    mux2 #(32) WRMUX( .sel(MemtoReg_WB), .a(talu_out_WB), .b(dmem_rdata_WB), .y(rfile_wd) ); 
				   
endmodule
