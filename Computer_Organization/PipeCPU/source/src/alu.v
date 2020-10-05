module alu( dataA, dataB, Signal, dataOut, zero );
	input [31:0] dataA ;
	input [31:0] dataB ;
	input [5:0] Signal ;
	output [31:0] dataOut ;
	output zero;

	//   Signal ( 6-bits)?
	//   AND  : 36
	//   OR   : 37
	//   ADD  : 32
	//   SUB  : 34
	//   SLT  : 42

	wire [31:0] ANDOut, OROut, FAOut, SLTOut;
	wire bin;
	wire [31:0] temp ;

	parameter AND = 6'b100100;
	parameter OR  = 6'b100101;
	parameter ADD = 6'b100000;
	parameter SUB = 6'b100010;
	parameter SLT = 6'b101010;
	/*
	定義各種訊號
	*/
	assign bin = (Signal == SUB | Signal == SLT) ? 1'b1 : 1'b0;
	RCALU RCALU(.dataA(dataA), .dataB(dataB), .Binvert(bin),
				.ANDOut(ANDOut), .OROut(OROut),
				.FAOut(FAOut), .SLTOut(SLTOut));

	assign dataOut = (Signal == AND) ? ANDOut :
					(Signal == OR) ? OROut :
					(Signal == ADD | Signal == SUB) ? FAOut :
					(Signal == SLT) ? SLTOut : 32'b0;
					
	assign zero = (dataOut == 32'd0) ? 1'b1 : 1'b0;
	
endmodule