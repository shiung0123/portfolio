
module RCALU( dataA, dataB, Binvert, ANDOut, OROut, FAOut, SLTOut );
	input [31:0] dataA ;
	input [31:0] dataB ;
	input Binvert;
	output [31:0] ANDOut, OROut, FAOut, SLTOut ;
	
	wire [31:0] c;
	wire set;
	assign set = FAOut[31] ;
	
	FALU tfalu_0(.a(dataA[0]), .b(dataB[0]), .less(set), .bin(Binvert), .c(Binvert),
				.cout(c[0]), .ANDOut(ANDOut[0]), .OROut(OROut[0]),
				.FAOut(FAOut[0]), .SLTOut(SLTOut[0]));

	FALU tfalu_1(.a(dataA[1]), .b(dataB[1]), .less(1'b0), .bin(Binvert), .c(c[0]),
				.cout(c[1]), .ANDOut(ANDOut[1]), .OROut(OROut[1]),
				.FAOut(FAOut[1]), .SLTOut(SLTOut[1]));

	FALU tfalu_2(.a(dataA[2]), .b(dataB[2]), .less(1'b0), .bin(Binvert), .c(c[1]),
				.cout(c[2]), .ANDOut(ANDOut[2]), .OROut(OROut[2]),
				.FAOut(FAOut[2]), .SLTOut(SLTOut[2]));

	FALU tfalu_3(.a(dataA[3]), .b(dataB[3]), .less(1'b0), .bin(Binvert), .c(c[2]),
				.cout(c[3]), .ANDOut(ANDOut[3]), .OROut(OROut[3]),
				.FAOut(FAOut[3]), .SLTOut(SLTOut[3]));

	FALU tfalu_4(.a(dataA[4]), .b(dataB[4]), .less(1'b0), .bin(Binvert), .c(c[3]),
				.cout(c[4]), .ANDOut(ANDOut[4]), .OROut(OROut[4]),
				.FAOut(FAOut[4]), .SLTOut(SLTOut[4]));

	FALU tfalu_5(.a(dataA[5]), .b(dataB[5]), .less(1'b0), .bin(Binvert), .c(c[4]),
				.cout(c[5]), .ANDOut(ANDOut[5]), .OROut(OROut[5]),
				.FAOut(FAOut[5]), .SLTOut(SLTOut[5]));

	FALU tfalu_6(.a(dataA[6]), .b(dataB[6]), .less(1'b0), .bin(Binvert), .c(c[5]),
				.cout(c[6]), .ANDOut(ANDOut[6]), .OROut(OROut[6]),
				.FAOut(FAOut[6]), .SLTOut(SLTOut[6]));

	FALU tfalu_7(.a(dataA[7]), .b(dataB[7]), .less(1'b0), .bin(Binvert), .c(c[6]),
				.cout(c[7]), .ANDOut(ANDOut[7]), .OROut(OROut[7]),
				.FAOut(FAOut[7]), .SLTOut(SLTOut[7]));

	FALU tfalu_8(.a(dataA[8]), .b(dataB[8]), .less(1'b0), .bin(Binvert), .c(c[7]),
				.cout(c[8]), .ANDOut(ANDOut[8]), .OROut(OROut[8]),
				.FAOut(FAOut[8]), .SLTOut(SLTOut[8]));

	FALU tfalu_9(.a(dataA[9]), .b(dataB[9]), .less(1'b0), .bin(Binvert), .c(c[8]),
				.cout(c[9]), .ANDOut(ANDOut[9]), .OROut(OROut[9]),
				.FAOut(FAOut[9]), .SLTOut(SLTOut[9]));

	FALU tfalu_10(.a(dataA[10]), .b(dataB[10]), .less(1'b0), .bin(Binvert), .c(c[9]),
				.cout(c[10]), .ANDOut(ANDOut[10]), .OROut(OROut[10]),
				.FAOut(FAOut[10]), .SLTOut(SLTOut[10]));

	FALU tfalu_11(.a(dataA[11]), .b(dataB[11]), .less(1'b0), .bin(Binvert), .c(c[10]),
				.cout(c[11]), .ANDOut(ANDOut[11]), .OROut(OROut[11]),
				.FAOut(FAOut[11]), .SLTOut(SLTOut[11]));

	FALU tfalu_12(.a(dataA[12]), .b(dataB[12]), .less(1'b0), .bin(Binvert), .c(c[11]),
				.cout(c[12]), .ANDOut(ANDOut[12]), .OROut(OROut[12]),
				.FAOut(FAOut[12]), .SLTOut(SLTOut[12]));

	FALU tfalu_13(.a(dataA[13]), .b(dataB[13]), .less(1'b0), .bin(Binvert), .c(c[12]),
				.cout(c[13]), .ANDOut(ANDOut[13]), .OROut(OROut[13]),
				.FAOut(FAOut[13]), .SLTOut(SLTOut[13]));

	FALU tfalu_14(.a(dataA[14]), .b(dataB[14]), .less(1'b0), .bin(Binvert), .c(c[13]),
				.cout(c[14]), .ANDOut(ANDOut[14]), .OROut(OROut[14]),
				.FAOut(FAOut[14]), .SLTOut(SLTOut[14]));

	FALU tfalu_15(.a(dataA[15]), .b(dataB[15]), .less(1'b0), .bin(Binvert), .c(c[14]),
				.cout(c[15]), .ANDOut(ANDOut[15]), .OROut(OROut[15]),
				.FAOut(FAOut[15]), .SLTOut(SLTOut[15]));

	FALU tfalu_16(.a(dataA[16]), .b(dataB[16]), .less(1'b0), .bin(Binvert), .c(c[15]),
				.cout(c[16]), .ANDOut(ANDOut[16]), .OROut(OROut[16]),
				.FAOut(FAOut[16]), .SLTOut(SLTOut[16]));

	FALU tfalu_17(.a(dataA[17]), .b(dataB[17]), .less(1'b0), .bin(Binvert), .c(c[16]),
				.cout(c[17]), .ANDOut(ANDOut[17]), .OROut(OROut[17]),
				.FAOut(FAOut[17]), .SLTOut(SLTOut[17]));

	FALU tfalu_18(.a(dataA[18]), .b(dataB[18]), .less(1'b0), .bin(Binvert), .c(c[17]),
				.cout(c[18]), .ANDOut(ANDOut[18]), .OROut(OROut[18]),
				.FAOut(FAOut[18]), .SLTOut(SLTOut[18]));

	FALU tfalu_19(.a(dataA[19]), .b(dataB[19]), .less(1'b0), .bin(Binvert), .c(c[18]),
				.cout(c[19]), .ANDOut(ANDOut[19]), .OROut(OROut[19]),
				.FAOut(FAOut[19]), .SLTOut(SLTOut[19]));

	FALU tfalu_20(.a(dataA[20]), .b(dataB[20]), .less(1'b0), .bin(Binvert), .c(c[19]),
				.cout(c[20]), .ANDOut(ANDOut[20]), .OROut(OROut[20]),
				.FAOut(FAOut[20]), .SLTOut(SLTOut[20]));

	FALU tfalu_21(.a(dataA[21]), .b(dataB[21]), .less(1'b0), .bin(Binvert), .c(c[20]),
				.cout(c[21]), .ANDOut(ANDOut[21]), .OROut(OROut[21]),
				.FAOut(FAOut[21]), .SLTOut(SLTOut[21]));

	FALU tfalu_22(.a(dataA[22]), .b(dataB[22]), .less(1'b0), .bin(Binvert), .c(c[21]),
				.cout(c[22]), .ANDOut(ANDOut[22]), .OROut(OROut[22]),
				.FAOut(FAOut[22]), .SLTOut(SLTOut[22]));

	FALU tfalu_23(.a(dataA[23]), .b(dataB[23]), .less(1'b0), .bin(Binvert), .c(c[22]),
				.cout(c[23]), .ANDOut(ANDOut[23]), .OROut(OROut[23]),
				.FAOut(FAOut[23]), .SLTOut(SLTOut[23]));

	FALU tfalu_24(.a(dataA[24]), .b(dataB[24]), .less(1'b0), .bin(Binvert), .c(c[23]),
				.cout(c[24]), .ANDOut(ANDOut[24]), .OROut(OROut[24]),
				.FAOut(FAOut[24]), .SLTOut(SLTOut[24]));

	FALU tfalu_25(.a(dataA[25]), .b(dataB[25]), .less(1'b0), .bin(Binvert), .c(c[24]),
				.cout(c[25]), .ANDOut(ANDOut[25]), .OROut(OROut[25]),
				.FAOut(FAOut[25]), .SLTOut(SLTOut[25]));

	FALU tfalu_26(.a(dataA[26]), .b(dataB[26]), .less(1'b0), .bin(Binvert), .c(c[25]),
				.cout(c[26]), .ANDOut(ANDOut[26]), .OROut(OROut[26]),
				.FAOut(FAOut[26]), .SLTOut(SLTOut[26]));

	FALU tfalu_27(.a(dataA[27]), .b(dataB[27]), .less(1'b0), .bin(Binvert), .c(c[26]),
				.cout(c[27]), .ANDOut(ANDOut[27]), .OROut(OROut[27]),
				.FAOut(FAOut[27]), .SLTOut(SLTOut[27]));

	FALU tfalu_28(.a(dataA[28]), .b(dataB[28]), .less(1'b0), .bin(Binvert), .c(c[27]),
				.cout(c[28]), .ANDOut(ANDOut[28]), .OROut(OROut[28]),
				.FAOut(FAOut[28]), .SLTOut(SLTOut[28]));

	FALU tfalu_29(.a(dataA[29]), .b(dataB[29]), .less(1'b0), .bin(Binvert), .c(c[28]),
				.cout(c[29]), .ANDOut(ANDOut[29]), .OROut(OROut[29]),
				.FAOut(FAOut[29]), .SLTOut(SLTOut[29]));

	FALU tfalu_30(.a(dataA[30]), .b(dataB[30]), .less(1'b0), .bin(Binvert), .c(c[29]),
				.cout(c[30]), .ANDOut(ANDOut[30]), .OROut(OROut[30]),
				.FAOut(FAOut[30]), .SLTOut(SLTOut[30]));

	FALU tfalu_31(.a(dataA[31]), .b(dataB[31]), .less(1'b0), .bin(Binvert), .c(c[30]),
				.cout(c[31]), .ANDOut(ANDOut[31]), .OROut(OROut[31]),
				.FAOut(FAOut[31]), .SLTOut(SLTOut[31]));


endmodule