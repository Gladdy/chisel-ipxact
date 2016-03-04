module Peripheral_0(input clk,
    input [31:0] io_in_data,
    input  io_write,
    output[31:0] io_out_data
);

  wire[31:0] T0;
  reg [31:0] R1;
  wire[31:0] T2;
  wire T3;

`ifndef SYNTHESIS
// synthesis translate_off
  integer initvar;
  initial begin
    #0.002;
    R1 = {1{$random}};
  end
// synthesis translate_on
`endif

  assign io_out_data = T0;
  assign T0 = T3 ? 32'h0 : R1;
  assign T2 = T3 ? io_in_data : R1;
  assign T3 = io_write == 1'h1;

  always @(posedge clk) begin
    if(T3) begin
      R1 <= io_in_data;
    end
  end
endmodule

module Peripheral_1(
    input [31:0] io_in_data,
    input  io_write,
    output[31:0] io_out_data
);



  assign io_out_data = 32'h539;
endmodule

module BasicBridge(input clk,
    input [31:0] io_in_data,
    input  io_write,
    input [31:0] io_address,
    output[31:0] io_out_data
);

  wire T0;
  wire T1;
  wire[31:0] T2;
  wire T3;
  wire T4;
  wire[31:0] T5;
  wire[31:0] T6;
  wire[31:0] T7;
  wire[31:0] Peripheral_io_out_data;
  wire[31:0] Peripheral_1_io_out_data;


  assign T0 = T1 ? io_write : 1'h0;
  assign T1 = io_address == 32'h4;
  assign T2 = T1 ? io_in_data : 32'h0;
  assign T3 = T4 ? io_write : 1'h0;
  assign T4 = io_address == 32'h0;
  assign T5 = T4 ? io_in_data : 32'h0;
  assign io_out_data = T6;
  assign T6 = T1 ? Peripheral_1_io_out_data : T7;
  assign T7 = T4 ? Peripheral_io_out_data : 32'h0;
  Peripheral_0 Peripheral(.clk(clk),
       .io_in_data( T5 ),
       .io_write( T3 ),
       .io_out_data( Peripheral_io_out_data )
  );
  Peripheral_1 Peripheral_1(
       .io_in_data( T2 ),
       .io_write( T0 ),
       .io_out_data( Peripheral_1_io_out_data )
  );
endmodule

