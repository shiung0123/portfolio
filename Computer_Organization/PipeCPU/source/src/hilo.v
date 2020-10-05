module hilo( clk, Ans, HiOut, LoOut );
	input clk ;
	input [63:0] Ans ;
	output [31:0] HiOut ;
	output [31:0] LoOut ;

	assign HiOut = Ans[63:32] ;
	assign LoOut = Ans[31:0] ;

endmodule