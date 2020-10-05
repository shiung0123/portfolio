module shifter( dataA, dataB, Signal, dataOut );

	input [31:0] dataA ; // DATA
	input [31:0] dataB ; // SHT
	input [5:0] Signal ;
	output [31:0] dataOut ;

	parameter SLL = 6'b000000;
	
	wire [31:0] temp[5:0];

	assign temp[0] = dataA;
	
	assign temp[1][0] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][0] :
						(-1 < 0) ? 1'b0 : temp[0][0] ) : 1'b0;

	assign temp[1][1] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][1] :
						(0 < 0) ? 1'b0 : temp[0][0] ) : 1'b0;

	assign temp[1][2] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][2] :
						(1 < 0) ? 1'b0 : temp[0][1] ) : 1'b0;

	assign temp[1][3] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][3] :
						(2 < 0) ? 1'b0 : temp[0][2] ) : 1'b0;

	assign temp[1][4] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][4] :
						(3 < 0) ? 1'b0 : temp[0][3] ) : 1'b0;

	assign temp[1][5] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][5] :
						(4 < 0) ? 1'b0 : temp[0][4] ) : 1'b0;

	assign temp[1][6] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][6] :
						(5 < 0) ? 1'b0 : temp[0][5] ) : 1'b0;

	assign temp[1][7] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][7] :
						(6 < 0) ? 1'b0 : temp[0][6] ) : 1'b0;

	assign temp[1][8] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][8] :
						(7 < 0) ? 1'b0 : temp[0][7] ) : 1'b0;

	assign temp[1][9] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][9] :
						(8 < 0) ? 1'b0 : temp[0][8] ) : 1'b0;

	assign temp[1][10] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][10] :
						(9 < 0) ? 1'b0 : temp[0][9] ) : 1'b0;

	assign temp[1][11] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][11] :
						(10 < 0) ? 1'b0 : temp[0][10] ) : 1'b0;

	assign temp[1][12] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][12] :
						(11 < 0) ? 1'b0 : temp[0][11] ) : 1'b0;

	assign temp[1][13] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][13] :
						(12 < 0) ? 1'b0 : temp[0][12] ) : 1'b0;

	assign temp[1][14] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][14] :
						(13 < 0) ? 1'b0 : temp[0][13] ) : 1'b0;

	assign temp[1][15] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][15] :
						(14 < 0) ? 1'b0 : temp[0][14] ) : 1'b0;

	assign temp[1][16] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][16] :
						(15 < 0) ? 1'b0 : temp[0][15] ) : 1'b0;

	assign temp[1][17] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][17] :
						(16 < 0) ? 1'b0 : temp[0][16] ) : 1'b0;

	assign temp[1][18] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][18] :
						(17 < 0) ? 1'b0 : temp[0][17] ) : 1'b0;

	assign temp[1][19] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][19] :
						(18 < 0) ? 1'b0 : temp[0][18] ) : 1'b0;

	assign temp[1][20] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][20] :
						(19 < 0) ? 1'b0 : temp[0][19] ) : 1'b0;

	assign temp[1][21] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][21] :
						(20 < 0) ? 1'b0 : temp[0][20] ) : 1'b0;

	assign temp[1][22] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][22] :
						(21 < 0) ? 1'b0 : temp[0][21] ) : 1'b0;

	assign temp[1][23] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][23] :
						(22 < 0) ? 1'b0 : temp[0][22] ) : 1'b0;

	assign temp[1][24] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][24] :
						(23 < 0) ? 1'b0 : temp[0][23] ) : 1'b0;

	assign temp[1][25] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][25] :
						(24 < 0) ? 1'b0 : temp[0][24] ) : 1'b0;

	assign temp[1][26] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][26] :
						(25 < 0) ? 1'b0 : temp[0][25] ) : 1'b0;

	assign temp[1][27] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][27] :
						(26 < 0) ? 1'b0 : temp[0][26] ) : 1'b0;

	assign temp[1][28] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][28] :
						(27 < 0) ? 1'b0 : temp[0][27] ) : 1'b0;

	assign temp[1][29] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][29] :
						(28 < 0) ? 1'b0 : temp[0][28] ) : 1'b0;

	assign temp[1][30] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][30] :
						(29 < 0) ? 1'b0 : temp[0][29] ) : 1'b0;

	assign temp[1][31] = (Signal == SLL) ?
						((~dataB[0]) ? temp[0][31] :
						(30 < 0) ? 1'b0 : temp[0][30] ) : 1'b0;

	assign temp[2][0] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][0] :
						(-2 < 0) ? 1'b0 : temp[1][0] ) : 1'b0;

	assign temp[2][1] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][1] :
						(-1 < 0) ? 1'b0 : temp[1][0] ) : 1'b0;

	assign temp[2][2] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][2] :
						(0 < 0) ? 1'b0 : temp[1][0] ) : 1'b0;

	assign temp[2][3] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][3] :
						(1 < 0) ? 1'b0 : temp[1][1] ) : 1'b0;

	assign temp[2][4] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][4] :
						(2 < 0) ? 1'b0 : temp[1][2] ) : 1'b0;

	assign temp[2][5] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][5] :
						(3 < 0) ? 1'b0 : temp[1][3] ) : 1'b0;

	assign temp[2][6] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][6] :
						(4 < 0) ? 1'b0 : temp[1][4] ) : 1'b0;

	assign temp[2][7] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][7] :
						(5 < 0) ? 1'b0 : temp[1][5] ) : 1'b0;

	assign temp[2][8] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][8] :
						(6 < 0) ? 1'b0 : temp[1][6] ) : 1'b0;

	assign temp[2][9] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][9] :
						(7 < 0) ? 1'b0 : temp[1][7] ) : 1'b0;

	assign temp[2][10] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][10] :
						(8 < 0) ? 1'b0 : temp[1][8] ) : 1'b0;

	assign temp[2][11] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][11] :
						(9 < 0) ? 1'b0 : temp[1][9] ) : 1'b0;

	assign temp[2][12] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][12] :
						(10 < 0) ? 1'b0 : temp[1][10] ) : 1'b0;

	assign temp[2][13] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][13] :
						(11 < 0) ? 1'b0 : temp[1][11] ) : 1'b0;

	assign temp[2][14] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][14] :
						(12 < 0) ? 1'b0 : temp[1][12] ) : 1'b0;

	assign temp[2][15] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][15] :
						(13 < 0) ? 1'b0 : temp[1][13] ) : 1'b0;

	assign temp[2][16] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][16] :
						(14 < 0) ? 1'b0 : temp[1][14] ) : 1'b0;

	assign temp[2][17] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][17] :
						(15 < 0) ? 1'b0 : temp[1][15] ) : 1'b0;

	assign temp[2][18] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][18] :
						(16 < 0) ? 1'b0 : temp[1][16] ) : 1'b0;

	assign temp[2][19] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][19] :
						(17 < 0) ? 1'b0 : temp[1][17] ) : 1'b0;

	assign temp[2][20] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][20] :
						(18 < 0) ? 1'b0 : temp[1][18] ) : 1'b0;

	assign temp[2][21] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][21] :
						(19 < 0) ? 1'b0 : temp[1][19] ) : 1'b0;

	assign temp[2][22] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][22] :
						(20 < 0) ? 1'b0 : temp[1][20] ) : 1'b0;

	assign temp[2][23] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][23] :
						(21 < 0) ? 1'b0 : temp[1][21] ) : 1'b0;

	assign temp[2][24] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][24] :
						(22 < 0) ? 1'b0 : temp[1][22] ) : 1'b0;

	assign temp[2][25] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][25] :
						(23 < 0) ? 1'b0 : temp[1][23] ) : 1'b0;

	assign temp[2][26] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][26] :
						(24 < 0) ? 1'b0 : temp[1][24] ) : 1'b0;

	assign temp[2][27] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][27] :
						(25 < 0) ? 1'b0 : temp[1][25] ) : 1'b0;

	assign temp[2][28] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][28] :
						(26 < 0) ? 1'b0 : temp[1][26] ) : 1'b0;

	assign temp[2][29] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][29] :
						(27 < 0) ? 1'b0 : temp[1][27] ) : 1'b0;

	assign temp[2][30] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][30] :
						(28 < 0) ? 1'b0 : temp[1][28] ) : 1'b0;

	assign temp[2][31] = (Signal == SLL) ?
						((~dataB[1]) ? temp[1][31] :
						(29 < 0) ? 1'b0 : temp[1][29] ) : 1'b0;

	assign temp[3][0] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][0] :
						(-4 < 0) ? 1'b0 : temp[2][0] ) : 1'b0;

	assign temp[3][1] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][1] :
						(-3 < 0) ? 1'b0 : temp[2][0] ) : 1'b0;

	assign temp[3][2] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][2] :
						(-2 < 0) ? 1'b0 : temp[2][0] ) : 1'b0;

	assign temp[3][3] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][3] :
						(-1 < 0) ? 1'b0 : temp[2][0] ) : 1'b0;

	assign temp[3][4] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][4] :
						(0 < 0) ? 1'b0 : temp[2][0] ) : 1'b0;

	assign temp[3][5] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][5] :
						(1 < 0) ? 1'b0 : temp[2][1] ) : 1'b0;

	assign temp[3][6] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][6] :
						(2 < 0) ? 1'b0 : temp[2][2] ) : 1'b0;

	assign temp[3][7] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][7] :
						(3 < 0) ? 1'b0 : temp[2][3] ) : 1'b0;

	assign temp[3][8] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][8] :
						(4 < 0) ? 1'b0 : temp[2][4] ) : 1'b0;

	assign temp[3][9] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][9] :
						(5 < 0) ? 1'b0 : temp[2][5] ) : 1'b0;

	assign temp[3][10] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][10] :
						(6 < 0) ? 1'b0 : temp[2][6] ) : 1'b0;

	assign temp[3][11] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][11] :
						(7 < 0) ? 1'b0 : temp[2][7] ) : 1'b0;

	assign temp[3][12] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][12] :
						(8 < 0) ? 1'b0 : temp[2][8] ) : 1'b0;

	assign temp[3][13] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][13] :
						(9 < 0) ? 1'b0 : temp[2][9] ) : 1'b0;

	assign temp[3][14] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][14] :
						(10 < 0) ? 1'b0 : temp[2][10] ) : 1'b0;

	assign temp[3][15] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][15] :
						(11 < 0) ? 1'b0 : temp[2][11] ) : 1'b0;

	assign temp[3][16] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][16] :
						(12 < 0) ? 1'b0 : temp[2][12] ) : 1'b0;

	assign temp[3][17] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][17] :
						(13 < 0) ? 1'b0 : temp[2][13] ) : 1'b0;

	assign temp[3][18] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][18] :
						(14 < 0) ? 1'b0 : temp[2][14] ) : 1'b0;

	assign temp[3][19] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][19] :
						(15 < 0) ? 1'b0 : temp[2][15] ) : 1'b0;

	assign temp[3][20] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][20] :
						(16 < 0) ? 1'b0 : temp[2][16] ) : 1'b0;

	assign temp[3][21] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][21] :
						(17 < 0) ? 1'b0 : temp[2][17] ) : 1'b0;

	assign temp[3][22] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][22] :
						(18 < 0) ? 1'b0 : temp[2][18] ) : 1'b0;

	assign temp[3][23] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][23] :
						(19 < 0) ? 1'b0 : temp[2][19] ) : 1'b0;

	assign temp[3][24] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][24] :
						(20 < 0) ? 1'b0 : temp[2][20] ) : 1'b0;

	assign temp[3][25] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][25] :
						(21 < 0) ? 1'b0 : temp[2][21] ) : 1'b0;

	assign temp[3][26] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][26] :
						(22 < 0) ? 1'b0 : temp[2][22] ) : 1'b0;

	assign temp[3][27] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][27] :
						(23 < 0) ? 1'b0 : temp[2][23] ) : 1'b0;

	assign temp[3][28] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][28] :
						(24 < 0) ? 1'b0 : temp[2][24] ) : 1'b0;

	assign temp[3][29] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][29] :
						(25 < 0) ? 1'b0 : temp[2][25] ) : 1'b0;

	assign temp[3][30] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][30] :
						(26 < 0) ? 1'b0 : temp[2][26] ) : 1'b0;

	assign temp[3][31] = (Signal == SLL) ?
						((~dataB[2]) ? temp[2][31] :
						(27 < 0) ? 1'b0 : temp[2][27] ) : 1'b0;

	assign temp[4][0] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][0] :
						(-8 < 0) ? 1'b0 : temp[3][0] ) : 1'b0;

	assign temp[4][1] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][1] :
						(-7 < 0) ? 1'b0 : temp[3][0] ) : 1'b0;

	assign temp[4][2] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][2] :
						(-6 < 0) ? 1'b0 : temp[3][0] ) : 1'b0;

	assign temp[4][3] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][3] :
						(-5 < 0) ? 1'b0 : temp[3][0] ) : 1'b0;

	assign temp[4][4] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][4] :
						(-4 < 0) ? 1'b0 : temp[3][0] ) : 1'b0;

	assign temp[4][5] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][5] :
						(-3 < 0) ? 1'b0 : temp[3][0] ) : 1'b0;

	assign temp[4][6] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][6] :
						(-2 < 0) ? 1'b0 : temp[3][0] ) : 1'b0;

	assign temp[4][7] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][7] :
						(-1 < 0) ? 1'b0 : temp[3][0] ) : 1'b0;

	assign temp[4][8] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][8] :
						(0 < 0) ? 1'b0 : temp[3][0] ) : 1'b0;

	assign temp[4][9] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][9] :
						(1 < 0) ? 1'b0 : temp[3][1] ) : 1'b0;

	assign temp[4][10] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][10] :
						(2 < 0) ? 1'b0 : temp[3][2] ) : 1'b0;

	assign temp[4][11] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][11] :
						(3 < 0) ? 1'b0 : temp[3][3] ) : 1'b0;

	assign temp[4][12] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][12] :
						(4 < 0) ? 1'b0 : temp[3][4] ) : 1'b0;

	assign temp[4][13] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][13] :
						(5 < 0) ? 1'b0 : temp[3][5] ) : 1'b0;

	assign temp[4][14] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][14] :
						(6 < 0) ? 1'b0 : temp[3][6] ) : 1'b0;

	assign temp[4][15] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][15] :
						(7 < 0) ? 1'b0 : temp[3][7] ) : 1'b0;

	assign temp[4][16] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][16] :
						(8 < 0) ? 1'b0 : temp[3][8] ) : 1'b0;

	assign temp[4][17] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][17] :
						(9 < 0) ? 1'b0 : temp[3][9] ) : 1'b0;

	assign temp[4][18] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][18] :
						(10 < 0) ? 1'b0 : temp[3][10] ) : 1'b0;

	assign temp[4][19] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][19] :
						(11 < 0) ? 1'b0 : temp[3][11] ) : 1'b0;

	assign temp[4][20] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][20] :
						(12 < 0) ? 1'b0 : temp[3][12] ) : 1'b0;

	assign temp[4][21] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][21] :
						(13 < 0) ? 1'b0 : temp[3][13] ) : 1'b0;

	assign temp[4][22] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][22] :
						(14 < 0) ? 1'b0 : temp[3][14] ) : 1'b0;

	assign temp[4][23] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][23] :
						(15 < 0) ? 1'b0 : temp[3][15] ) : 1'b0;

	assign temp[4][24] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][24] :
						(16 < 0) ? 1'b0 : temp[3][16] ) : 1'b0;

	assign temp[4][25] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][25] :
						(17 < 0) ? 1'b0 : temp[3][17] ) : 1'b0;

	assign temp[4][26] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][26] :
						(18 < 0) ? 1'b0 : temp[3][18] ) : 1'b0;

	assign temp[4][27] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][27] :
						(19 < 0) ? 1'b0 : temp[3][19] ) : 1'b0;

	assign temp[4][28] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][28] :
						(20 < 0) ? 1'b0 : temp[3][20] ) : 1'b0;

	assign temp[4][29] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][29] :
						(21 < 0) ? 1'b0 : temp[3][21] ) : 1'b0;

	assign temp[4][30] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][30] :
						(22 < 0) ? 1'b0 : temp[3][22] ) : 1'b0;

	assign temp[4][31] = (Signal == SLL) ?
						((~dataB[3]) ? temp[3][31] :
						(23 < 0) ? 1'b0 : temp[3][23] ) : 1'b0;

	assign temp[5][0] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][0] :
						(-16 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][1] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][1] :
						(-15 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][2] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][2] :
						(-14 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][3] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][3] :
						(-13 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][4] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][4] :
						(-12 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][5] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][5] :
						(-11 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][6] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][6] :
						(-10 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][7] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][7] :
						(-9 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][8] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][8] :
						(-8 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][9] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][9] :
						(-7 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][10] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][10] :
						(-6 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][11] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][11] :
						(-5 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][12] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][12] :
						(-4 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][13] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][13] :
						(-3 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][14] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][14] :
						(-2 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][15] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][15] :
						(-1 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][16] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][16] :
						(0 < 0) ? 1'b0 : temp[4][0] ) : 1'b0;

	assign temp[5][17] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][17] :
						(1 < 0) ? 1'b0 : temp[4][1] ) : 1'b0;

	assign temp[5][18] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][18] :
						(2 < 0) ? 1'b0 : temp[4][2] ) : 1'b0;

	assign temp[5][19] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][19] :
						(3 < 0) ? 1'b0 : temp[4][3] ) : 1'b0;

	assign temp[5][20] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][20] :
						(4 < 0) ? 1'b0 : temp[4][4] ) : 1'b0;

	assign temp[5][21] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][21] :
						(5 < 0) ? 1'b0 : temp[4][5] ) : 1'b0;

	assign temp[5][22] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][22] :
						(6 < 0) ? 1'b0 : temp[4][6] ) : 1'b0;

	assign temp[5][23] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][23] :
						(7 < 0) ? 1'b0 : temp[4][7] ) : 1'b0;

	assign temp[5][24] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][24] :
						(8 < 0) ? 1'b0 : temp[4][8] ) : 1'b0;

	assign temp[5][25] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][25] :
						(9 < 0) ? 1'b0 : temp[4][9] ) : 1'b0;

	assign temp[5][26] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][26] :
						(10 < 0) ? 1'b0 : temp[4][10] ) : 1'b0;

	assign temp[5][27] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][27] :
						(11 < 0) ? 1'b0 : temp[4][11] ) : 1'b0;

	assign temp[5][28] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][28] :
						(12 < 0) ? 1'b0 : temp[4][12] ) : 1'b0;

	assign temp[5][29] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][29] :
						(13 < 0) ? 1'b0 : temp[4][13] ) : 1'b0;

	assign temp[5][30] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][30] :
						(14 < 0) ? 1'b0 : temp[4][14] ) : 1'b0;

	assign temp[5][31] = (Signal == SLL) ?
						((~dataB[4]) ? temp[4][31] :
						(15 < 0) ? 1'b0 : temp[4][15] ) : 1'b0;

	assign dataOut = temp[5];

endmodule