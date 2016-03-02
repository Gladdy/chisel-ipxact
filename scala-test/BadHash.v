module BadHash(
    input [31:0] io_in_data,
    output[31:0] io_out_data
);

  wire[31:0] T7;
  wire T0;
  wire T1;
  wire T2;
  wire T3;
  wire T4;
  wire T5;
  wire T6;


  assign io_out_data = T7;
  assign T7 = {31'h0, T0};
  assign T0 = T6 ? 1'h0 : T1;
  assign T1 = T5 ? 1'h1 : T2;
  assign T2 = T4 ? 1'h0 : T3;
  assign T3 = io_in_data == 32'h0;
  assign T4 = T3 ^ 1'h1;
  assign T5 = io_in_data == 32'h4;
  assign T6 = T5 ^ 1'h1;
endmodule

