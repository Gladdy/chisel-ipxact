module Counter(input clk, input reset,
    input  io_inc,
    input [3:0] io_amt,
    output[7:0] io_tot
);

  reg [7:0] R0;
  wire[7:0] T5;
  wire[7:0] T1;
  wire[7:0] T2;
  wire[7:0] T3;
  wire[7:0] T6;
  wire T4;

`ifndef SYNTHESIS
// synthesis translate_off
  integer initvar;
  initial begin
    #0.002;
    R0 = {1{$random}};
  end
// synthesis translate_on
`endif

  assign io_tot = R0;
  assign T5 = reset ? 8'h0 : T1;
  assign T1 = io_inc ? T2 : R0;
  assign T2 = T4 ? 8'h0 : T3;
  assign T3 = R0 + T6;
  assign T6 = {4'h0, io_amt};
  assign T4 = 8'hff < T3;

  always @(posedge clk) begin
    if(reset) begin
      R0 <= 8'h0;
    end else if(io_inc) begin
      R0 <= T2;
    end
  end
endmodule

