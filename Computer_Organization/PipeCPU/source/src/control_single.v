/*
	Title: MIPS Single-Cycle Control Unit
	Editor: Selene (Computer System and Architecture Lab, ICE, CYCU)
	
	Input Port
		1. opcode: 輸入的指令代號，據此產生對應的控制訊號
	Input Port
		1. RegDst: 控制RFMUX
		2. ALUSrc: 控制ALUMUX
		3. MemtoReg: 控制WRMUX
		4. RegWrite: 控制暫存器是否可寫入
		5. MemRead:  控制記憶體是否可讀出
		6. MemWrite: 控制記憶體是否可寫入
		7. Branch: 與ALU輸出的zero訊號做AND運算控制PCMUX
		8. ALUOp: 輸出至ALU Control
*/
module control_single( instr, RegDst, RegWrite, ALUSrc,
					   MemRead, MemWrite, MemtoReg,
					   Branch_EQ, Branch_NE, Jump, ALUOp );
    input[31:0] instr;
    output RegDst, RegWrite, ALUSrc, MemRead, MemWrite, MemtoReg, Branch_EQ, Branch_NE, Jump;
    output[1:0] ALUOp;
    reg RegDst, RegWrite, ALUSrc, MemRead, MemWrite, MemtoReg, Branch_EQ, Branch_NE, Jump;
    reg[1:0] ALUOp;

	parameter NOP = 32'd0;
	parameter MULTU = 6'd25;
	parameter MFHI = 6'd16;
	parameter MFLO = 6'd18;
	parameter SLL = 6'd0;
    parameter R_FORMAT = 6'd0;
	parameter ADDIU = 6'd9;
    parameter LW = 6'd35;
    parameter SW = 6'd43;
    parameter BEQ = 6'd4;
	parameter BNE = 6'd5;
	parameter J = 6'd2;
	
	wire[5:0] opcode;
	wire[5:0] func;
	assign opcode = instr[31:26];
	assign func = instr[5:0];

    always @( instr ) begin
		if (instr == NOP)
		begin
			RegDst = 1'bx; RegWrite = 1'b0; ALUSrc = 1'bx;
			MemRead = 1'b0; MemWrite = 1'b0; MemtoReg = 1'bx;
			Branch_EQ = 1'b0; Branch_NE = 1'b0; Jump = 1'b0; ALUOp = 2'bxx;
		end
        else
		case ( opcode )
			R_FORMAT : 
			case (func)
				MULTU :
					begin
					RegDst = 1'bx; RegWrite = 1'b0; ALUSrc = 1'bx;
					MemRead = 1'b0; MemWrite = 1'b0; MemtoReg = 1'bx;
					Branch_EQ = 1'b0; Branch_NE = 1'b0; Jump = 1'b0; ALUOp = 2'b10;
					end
				MFHI | MFLO | SLL :
					begin
					RegDst = 1'b0; RegWrite = 1'b1; ALUSrc = 1'bx;
					MemRead = 1'b0; MemWrite = 1'b0; MemtoReg = 1'b0;
					Branch_EQ = 1'b0; Branch_NE = 1'b0; Jump = 1'b0; ALUOp = 2'b10;
					end
				default
				begin
					RegDst = 1'b0; RegWrite = 1'b1; ALUSrc = 1'b0;
					MemRead = 1'b0; MemWrite = 1'b0; MemtoReg = 1'b0;
					Branch_EQ = 1'b0; Branch_NE = 1'b0; Jump = 1'b0; ALUOp = 2'b10;
				end
			endcase
			ADDIU :
			begin
				RegDst = 1'b1; RegWrite = 1'b1; ALUSrc = 1'b1;
				MemRead = 1'b0; MemWrite = 1'b0; MemtoReg = 1'b0;
				Branch_EQ = 1'b0; Branch_NE = 1'b0; Jump = 1'b0; ALUOp = 2'b00;
			end
			LW :
			begin
				RegDst = 1'b1; RegWrite = 1'b1; ALUSrc = 1'b1;
				MemRead = 1'b1; MemWrite = 1'b0; MemtoReg = 1'b1;
				Branch_EQ = 1'b0; Branch_NE = 1'b0; Jump = 1'b0; ALUOp = 2'b00;
			end
			SW :
			begin
				RegDst = 1'bx; RegWrite = 1'b0; ALUSrc = 1'b1;
				MemRead = 1'b0; MemWrite = 1'b1; MemtoReg = 1'bx;
				Branch_EQ = 1'b0; Branch_NE = 1'b0; Jump = 1'b0; ALUOp = 2'b00;
			end
			BEQ :
			begin
				RegDst = 1'bx; RegWrite = 1'b0; ALUSrc = 1'b0;
				MemRead = 1'b0; MemWrite = 1'b0; MemtoReg = 1'bx;
				Branch_EQ = 1'b1; Branch_NE = 1'b0; Jump = 1'b0; ALUOp = 2'b01;
			end
			BNE :
			begin
				RegDst = 1'bx; RegWrite = 1'b0; ALUSrc = 1'b0;
				MemRead = 1'b0; MemWrite = 1'b0; MemtoReg = 1'bx;
				Branch_EQ = 1'b0; Branch_NE = 1'b1; Jump = 1'b0; ALUOp = 2'b01;
			end
			J :
			begin
				RegDst = 1'bx; RegWrite = 1'b0; ALUSrc = 1'bx;
				MemRead = 1'b0; MemWrite = 1'b0; MemtoReg = 1'bx;
				Branch_EQ = 1'b0; Branch_NE = 1'b0; Jump = 1'b1; ALUOp = 2'bxx;
			end
			default
			begin
				$display("control_single unimplemented opcode %d", opcode);
				RegDst = 1'bx; RegWrite = 1'bx; ALUSrc = 1'bx;
				MemRead = 1'bx; MemWrite = 1'bx; MemtoReg = 1'bx;
				Branch_EQ = 1'bx; Branch_NE = 1'bx; Jump = 1'bx; ALUOp = 2'bxx;
			end
        endcase
    end
endmodule

