
module FALU( a, b, less, bin, c, cout, ANDOut, OROut, FAOut, SLTOut );
	input a, b, less, bin, c;
	output cout, ANDOut, OROut, FAOut, SLTOut ;
	
	wire   fa_b;
	
	and (ANDOut, a, b);
	
	or (OROut, a, b);
	
	xor (fa_b, b, bin);
	FA FA(.a(a), .b(fa_b), .c(c), .cout(cout), .sum(FAOut));
	
	assign SLTOut = less;

endmodule