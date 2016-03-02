module BadHash(input clk, input reset,
    input [31:0] io_in_data,
    input  io_in_ready,
    output[31:0] io_out_data,
    output[31:0] io_out_ready
);

  wire[31:0] T11;
  wire T0;
  wire T1;
  reg  running;
  wire T12;
  wire T2;
  wire T3;
  wire T4;
  wire T5;
  wire T6;
  wire T7;
  wire[31:0] T8;
  wire[31:0] T9;
  reg [31:0] operating_register;
  wire[31:0] T13;
  wire[31:0] T10;

`ifndef SYNTHESIS
// synthesis translate_off
  integer initvar;
  initial begin
    #0.002;
    running = {1{$random}};
    operating_register = {1{$random}};
  end
// synthesis translate_on
`endif

  assign io_out_ready = T11;
  assign T11 = {31'h0, T0};
  assign T0 = T7 & T1;
  assign T1 = running == 1'h1;
  assign T12 = reset ? 1'h0 : T2;
  assign T2 = T0 ? 1'h0 : T3;
  assign T3 = T4 ? 1'h1 : running;
  assign T4 = T6 & T5;
  assign T5 = running == 1'h0;
  assign T6 = io_in_ready == 1'h1;
  assign T7 = T4 ^ 1'h1;
  assign io_out_data = T8;
  assign T8 = T0 ? T9 : 32'h0;
  assign T9 = operating_register ^ 32'h1ea14969;
  assign T13 = reset ? 32'h0 : T10;
  assign T10 = T4 ? io_in_data : operating_register;

  always @(posedge clk) begin
    if(reset) begin
      running <= 1'h0;
    end else if(T0) begin
      running <= 1'h0;
    end else if(T4) begin
      running <= 1'h1;
    end
    if(reset) begin
      operating_register <= 32'h0;
    end else if(T4) begin
      operating_register <= io_in_data;
    end
  end
endmodule

