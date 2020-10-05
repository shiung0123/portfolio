/*
	Title: 32-Bit Register with Synchronous Reset
	Editor: Selene (Computer System and Architecture Lab, ICE, CYCU)
	
	Input Port
		1. clk
		2. rst: 重置訊號
		3. en_reg: 控制暫存器是否可寫入
		4. d_in: 欲寫入的暫存器資料
	Output Port
		1. d_out: 所讀取的暫存器資料
*/
module reg_pipe ( clk, rst, en_w,
				  r32_in1, r32_in2, r32_in3, r32_in4, r32_in5,
				  r32_out1, r32_out2, r32_out3, r32_out4, r32_out5,
				  r5_in1, r5_in2, r26_in1, r2_in1, r5_out1, r5_out2, r26_out1, r2_out1,
				  e_in1, e_in2, e_in3, e_in4, e_in5, m_in1, m_in2, w_in1, w_in2,
				  e_out1, e_out2, e_out3, e_out4, e_out5, m_out1, m_out2, w_out1, w_out2 );
				  
    input clk, rst, en_w;
	
    input[31:0]	r32_in1, r32_in2, r32_in3, r32_in4, r32_in5;
	input[4:0] r5_in1, r5_in2;
	input[25:0]	r26_in1;
	input[1:0] r2_in1;
	input e_in1, e_in2, e_in3, e_in4, e_in5;
	input m_in1, m_in2;
	input w_in1, w_in2;
	
    output[31:0] r32_out1, r32_out2, r32_out3, r32_out4, r32_out5;
	output[4:0]	r5_out1, r5_out2;
	output[25:0] r26_out1;
	output[1:0] r2_out1;
	output e_out1, e_out2, e_out3, e_out4, e_out5;
	output m_out1, m_out2;
	output w_out1, w_out2;
	
    reg[31:0] r32_out1, r32_out2, r32_out3, r32_out4, r32_out5;
	reg[4:0] r5_out1, r5_out2;
	reg[25:0] r26_out1;
	reg[1:0] r2_out1;
	reg e_out1, e_out2, e_out3, e_out4, e_out5;
	reg m_out1, m_out2;
	reg w_out1, w_out2;
   
    always @( posedge clk ) begin
        if ( rst )
		begin
			r32_out1 <= 32'b0;
			r32_out2 <= 32'b0;
			r32_out3 <= 32'b0;
			r32_out4 <= 32'b0;
			r32_out5 <= 32'b0;
			r5_out1 <= 5'b0;
			r5_out2 <= 5'b0;
			r26_out1 <= 26'b0;
			r2_out1 <= 26'b0;
			e_out1 <= 1'b0;
			e_out2 <= 1'b0;
			e_out3 <= 1'b0;
			e_out4 <= 1'b0;
			e_out5 <= 1'b0;
			m_out1 <= 1'b0;
			m_out2 <= 1'b0;
			w_out1 <= 1'b0;
			w_out2 <= 1'b0;
		end
        else if ( en_w )
		begin
			r32_out1 <= r32_in1;
			r32_out2 <= r32_in2;
			r32_out3 <= r32_in3;
			r32_out4 <= r32_in4;
			r32_out5 <= r32_in5;
			r5_out1 <= r5_in1;
			r5_out2 <= r5_in2;
			r26_out1 <= r26_in1;
			r2_out1 <= r2_in1;
			e_out1 <= e_in1;
			e_out2 <= e_in2;
			e_out3 <= e_in3;
			e_out4 <= e_in4;
			e_out5 <= e_in5;
			m_out1 <= m_in1;
			m_out2 <= m_in2;
			w_out1 <= w_in1;
			w_out2 <= w_in2;
		end	
    end

endmodule
	
