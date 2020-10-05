module multiplier( clk, dataA, dataB, Signal, dataOut );
	input clk ;
	input [31:0] dataA ;
	input [31:0] dataB ;
	input [5:0] Signal ;
	output [63:0] dataOut ;

	//   Signal ( 7-bits)?
	//   init
	//   DIVU  : 25
	
	reg [63:0] a ;
	reg [31:0] b ;
	reg [63:0] hilo ;
	reg [63:0] temp ;
	reg [6:0] counter ;
	parameter MULTU = 6'b011001;
	parameter OUT = 6'b111111;
	/*
	定義各種訊號
	*/
	
	always@( posedge clk or Signal )
	begin
		if ( Signal == MULTU )
		begin
			counter = 31;
			a = dataA ;
			b = dataB ;
			temp = 64'b0;
		end
		if ( counter >= 0 & counter <= 31 )
		begin
			temp = temp + a * b[0];
			a = a << 1;
			b = b >> 1;
			counter = counter - 1 ;
		end
		else hilo = temp;
	end
		
	/*
	數32個clk然後開啟HiLo暫存器給除法器放值進去
	cycle 0  counter 0->1   : set a b, do bit 0
	cycle 1  counter 1->2   : get bit 0 ans , do bit 1
	cycle 31 counter 31->32 : get bit 30 ans , do bit 31
	cycle 32 counter 32->33 : get bit 31 ans , switch OUT
	total need 33 cycles
	*/

	/*
	乘法運算
	OUT的部分是要等control給你指令你才能夠把答案輸出到HILO暫存器
	*/
	
	assign dataOut = hilo ;

endmodule