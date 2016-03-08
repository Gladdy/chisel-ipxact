#include "IPBlock.h"

void IPBlock_t::init ( val_t rand_init ) {
  this->__srand(rand_init);
  IPBlock_pb__out.randomize(&__rand_seed);
  IPBlock_pa__out.randomize(&__rand_seed);
  IPBlock_s__wvalid.randomize(&__rand_seed);
  IPBlock_s__wresp_valid.randomize(&__rand_seed);
  IPBlock_s__rvalid.randomize(&__rand_seed);
  IPBlock_s__addr.randomize(&__rand_seed);
  clk.len = 1;
  clk.cnt = 0;
  clk.values[0] = 0;
}


int IPBlock_t::clock ( dat_t<1> reset ) {
  uint32_t min = ((uint32_t)1<<31)-1;
  if (clk.cnt < min) min = clk.cnt;
  clk.cnt-=min;
  if (clk.cnt == 0) clock_lo( reset );
  if (clk.cnt == 0) clock_hi( reset );
  if (clk.cnt == 0) clk.cnt = clk.len;
  return min;
}


void IPBlock_t::print ( FILE* f ) {
}
void IPBlock_t::print ( std::ostream& s ) {
}


void IPBlock_t::dump_init ( FILE* f ) {
}


void IPBlock_t::dump ( FILE* f, int t, dat_t<1> reset ) {
}




void IPBlock_t::clock_lo ( dat_t<1> reset, bool assert_fire ) {
  val_t T0;
  T0 = IPBlock_s__addr.values[0] == 0x0L;
  val_t T1;
  { T1 = TERNARY(T0, 0x0L, 0x0L);}
  val_t T2;
  { T2 = T1 | 0x0L << 1;}
  { IPBlock_s__io_axi_wchannel_bits_wdata.values[0] = IPBlock__io_wchannel_bits_wdata.values[0];}
  val_t T3;
  T3 = IPBlock_s__addr.values[0] == 0x4L;
  val_t T4;
  { T4 = T0 ^ 0x1L;}
  val_t T5;
  { T5 = T4 & T3;}
  val_t T6;
  { T6 = TERNARY_1(T5, IPBlock_s__io_axi_wchannel_bits_wdata.values[0], T2);}
  { IPBlock_s__io_slaveB_in_bits_wdata.values[0] = T6;}
  { IPBlock_pb__io_in_bits_wdata.values[0] = IPBlock_s__io_slaveB_in_bits_wdata.values[0];}
  val_t T7;
  { T7 = TERNARY(T0, 0x0L, 0x0L);}
  { IPBlock_s__io_axi_wchannel_valid.values[0] = IPBlock__io_wchannel_valid.values[0];}
  val_t T8;
  { T8 = TERNARY_1(T5, IPBlock_s__io_axi_wchannel_valid.values[0], T7);}
  { IPBlock_s__io_slaveB_in_valid.values[0] = T8;}
  { IPBlock_pb__io_in_valid.values[0] = IPBlock_s__io_slaveB_in_valid.values[0];}
  val_t T9;
  { T9 = TERNARY_1(IPBlock_pb__io_in_valid.values[0], IPBlock_pb__io_in_bits_wdata.values[0], IPBlock_pb__out.values[0]);}
  { IPBlock_pb__reset.values[0] = reset.values[0];}
  { T10.values[0] = TERNARY(IPBlock_pb__reset.values[0], 0xbeefL, T9);}
  val_t T11;
  { T11 = TERNARY(T0, IPBlock_s__io_axi_wchannel_bits_wdata.values[0], 0x0L);}
  val_t T12;
  { T12 = TERNARY(T5, 0x0L, T11);}
  { IPBlock_s__io_slaveA_in_bits_wdata.values[0] = T12;}
  { IPBlock_pa__io_in_bits_wdata.values[0] = IPBlock_s__io_slaveA_in_bits_wdata.values[0];}
  val_t T13;
  { T13 = TERNARY(T0, IPBlock_s__io_axi_wchannel_valid.values[0], 0x0L);}
  val_t T14;
  { T14 = TERNARY(T5, 0x0L, T13);}
  { IPBlock_s__io_slaveA_in_valid.values[0] = T14;}
  { IPBlock_pa__io_in_valid.values[0] = IPBlock_s__io_slaveA_in_valid.values[0];}
  val_t T15;
  { T15 = TERNARY_1(IPBlock_pa__io_in_valid.values[0], IPBlock_pa__io_in_bits_wdata.values[0], IPBlock_pa__out.values[0]);}
  { IPBlock_pa__reset.values[0] = reset.values[0];}
  { T16.values[0] = TERNARY(IPBlock_pa__reset.values[0], 0x7eadbeefL, T15);}
  val_t T17;
  { T17 = IPBlock_s__wvalid.values[0] ^ 0x1L;}
  { IPBlock_s__io_axi_awchannel_ready.values[0] = T17;}
  { IPBlock_s__io_axi_awchannel_valid.values[0] = IPBlock__io_awchannel_valid.values[0];}
  val_t T18;
  { T18 = IPBlock_s__io_axi_awchannel_valid.values[0] & IPBlock_s__io_axi_awchannel_ready.values[0];}
  val_t T19;
  { T19 = TERNARY(T18, 0x1L, IPBlock_s__wvalid.values[0]);}
  val_t T20;
  { T20 = IPBlock_s__wresp_valid.values[0] ^ 0x1L;}
  { IPBlock_pa__io_in_ready.values[0] = 0x1L;}
  { IPBlock_s__io_slaveA_in_ready.values[0] = IPBlock_pa__io_in_ready.values[0];}
  val_t T21;
  { T21 = IPBlock_s__io_slaveA_in_ready.values[0] & IPBlock_s__wvalid.values[0];}
  val_t T22;
  { T22 = T21 & T20;}
  val_t T23;
  { T23 = TERNARY(T0, T22, 0x0L);}
  val_t T24;
  { T24 = IPBlock_s__wresp_valid.values[0] ^ 0x1L;}
  { IPBlock_pb__io_in_ready.values[0] = 0x1L;}
  { IPBlock_s__io_slaveB_in_ready.values[0] = IPBlock_pb__io_in_ready.values[0];}
  val_t T25;
  { T25 = IPBlock_s__io_slaveB_in_ready.values[0] & IPBlock_s__wvalid.values[0];}
  val_t T26;
  { T26 = T25 & T24;}
  val_t T27;
  { T27 = TERNARY_1(T5, T26, T23);}
  { IPBlock_s__io_axi_wchannel_ready.values[0] = T27;}
  val_t T28;
  { T28 = IPBlock_s__io_axi_wchannel_valid.values[0] & IPBlock_s__io_axi_wchannel_ready.values[0];}
  val_t T29;
  { T29 = TERNARY(T28, 0x0L, T19);}
  { IPBlock_s__reset.values[0] = reset.values[0];}
  { T30.values[0] = TERNARY(IPBlock_s__reset.values[0], 0x0L, T29);}
  val_t T31;
  { T31 = IPBlock_s__io_axi_wchannel_valid.values[0] & IPBlock_s__io_axi_wchannel_ready.values[0];}
  val_t T32;
  { T32 = TERNARY(T31, 0x1L, IPBlock_s__wresp_valid.values[0]);}
  { IPBlock_s__io_axi_bchannel_ready.values[0] = IPBlock__io_bchannel_ready.values[0];}
  { IPBlock_s__io_axi_bchannel_valid.values[0] = IPBlock_s__wresp_valid.values[0];}
  val_t T33;
  { T33 = IPBlock_s__io_axi_bchannel_valid.values[0] & IPBlock_s__io_axi_bchannel_ready.values[0];}
  val_t T34;
  { T34 = TERNARY(T33, 0x0L, T32);}
  { T35.values[0] = TERNARY(IPBlock_s__reset.values[0], 0x0L, T34);}
  val_t T36;
  { T36 = IPBlock_s__rvalid.values[0] ^ 0x1L;}
  { IPBlock_s__io_axi_archannel_ready.values[0] = T36;}
  { IPBlock_s__io_axi_archannel_valid.values[0] = IPBlock__io_archannel_valid.values[0];}
  val_t T37;
  { T37 = IPBlock_s__io_axi_archannel_valid.values[0] & IPBlock_s__io_axi_archannel_ready.values[0];}
  val_t T38;
  { T38 = TERNARY(T37, 0x1L, IPBlock_s__rvalid.values[0]);}
  { IPBlock_s__io_axi_rchannel_ready.values[0] = IPBlock__io_rchannel_ready.values[0];}
  { IPBlock_pa__io_out_valid.values[0] = 0x1L;}
  { IPBlock_s__io_slaveA_out_valid.values[0] = IPBlock_pa__io_out_valid.values[0];}
  val_t T39;
  { T39 = IPBlock_s__io_slaveA_out_valid.values[0] & IPBlock_s__rvalid.values[0];}
  val_t T40;
  T40 = IPBlock_s__addr.values[0] == 0x0L;
  val_t T41;
  { T41 = TERNARY(T40, T39, 0x0L);}
  { IPBlock_pb__io_out_valid.values[0] = 0x1L;}
  { IPBlock_s__io_slaveB_out_valid.values[0] = IPBlock_pb__io_out_valid.values[0];}
  val_t T42;
  { T42 = IPBlock_s__io_slaveB_out_valid.values[0] & IPBlock_s__rvalid.values[0];}
  val_t T43;
  T43 = IPBlock_s__addr.values[0] == 0x4L;
  val_t T44;
  { T44 = T40 ^ 0x1L;}
  val_t T45;
  { T45 = T44 & T43;}
  val_t T46;
  { T46 = TERNARY_1(T45, T42, T41);}
  { IPBlock_s__io_axi_rchannel_valid.values[0] = T46;}
  val_t T47;
  { T47 = IPBlock_s__io_axi_rchannel_valid.values[0] & IPBlock_s__io_axi_rchannel_ready.values[0];}
  val_t T48;
  { T48 = TERNARY(T47, 0x0L, T38);}
  { T49.values[0] = TERNARY(IPBlock_s__reset.values[0], 0x0L, T48);}
  { val_t __r = this->__rand_val(); IPBlock_s__io_axi_rchannel_bits_rresp.values[0] = __r;}
  IPBlock_s__io_axi_rchannel_bits_rresp.values[0] = IPBlock_s__io_axi_rchannel_bits_rresp.values[0] & 0x3L;
  { IPBlock_s__io_axi_archannel_bits_araddr.values[0] = IPBlock__io_archannel_bits_araddr.values[0];}
  val_t T50;
  { T50 = TERNARY_1(T37, IPBlock_s__io_axi_archannel_bits_araddr.values[0], IPBlock_s__addr.values[0]);}
  { IPBlock_s__io_axi_awchannel_bits_awaddr.values[0] = IPBlock__io_awchannel_bits_awaddr.values[0];}
  val_t T51;
  { T51 = TERNARY_1(T18, IPBlock_s__io_axi_awchannel_bits_awaddr.values[0], T50);}
  { T52.values[0] = TERNARY(IPBlock_s__reset.values[0], 0x0L, T51);}
  { IPBlock__io_awchannel_ready.values[0] = IPBlock_s__io_axi_awchannel_ready.values[0];}
  { IPBlock__io_wchannel_ready.values[0] = IPBlock_s__io_axi_wchannel_ready.values[0];}
  { IPBlock__io_bchannel_valid.values[0] = IPBlock_s__io_axi_bchannel_valid.values[0];}
  { IPBlock_s__io_axi_bchannel_bits_bresp.values[0] = 0x0L;}
  { IPBlock__io_bchannel_bits_bresp.values[0] = IPBlock_s__io_axi_bchannel_bits_bresp.values[0];}
  { IPBlock__io_archannel_ready.values[0] = IPBlock_s__io_axi_archannel_ready.values[0];}
  { IPBlock__io_rchannel_valid.values[0] = IPBlock_s__io_axi_rchannel_valid.values[0];}
  { IPBlock_pa__io_out_bits_rdata.values[0] = IPBlock_pa__out.values[0];}
  { IPBlock_s__io_slaveA_out_bits_rdata.values[0] = IPBlock_pa__io_out_bits_rdata.values[0];}
  val_t T53;
  { T53 = TERNARY(T40, IPBlock_s__io_slaveA_out_bits_rdata.values[0], 0x0L);}
  { IPBlock_pb__io_out_bits_rdata.values[0] = IPBlock_pb__out.values[0];}
  { IPBlock_s__io_slaveB_out_bits_rdata.values[0] = IPBlock_pb__io_out_bits_rdata.values[0];}
  val_t T54;
  { T54 = TERNARY_1(T45, IPBlock_s__io_slaveB_out_bits_rdata.values[0], T53);}
  { IPBlock_s__io_axi_rchannel_bits_rdata.values[0] = T54;}
  { IPBlock__io_rchannel_bits_rdata.values[0] = IPBlock_s__io_axi_rchannel_bits_rdata.values[0];}
  { val_t __r = this->__rand_val(); IPBlock__io_rchannel_bits_rresp.values[0] = __r;}
  IPBlock__io_rchannel_bits_rresp.values[0] = IPBlock__io_rchannel_bits_rresp.values[0] & 0x3L;
}


void IPBlock_t::clock_hi ( dat_t<1> reset ) {
  dat_t<32> IPBlock_pb__out__shadow = T10;
  dat_t<32> IPBlock_pa__out__shadow = T16;
  dat_t<1> IPBlock_s__wvalid__shadow = T30;
  dat_t<1> IPBlock_s__wresp_valid__shadow = T35;
  dat_t<1> IPBlock_s__rvalid__shadow = T49;
  dat_t<32> IPBlock_s__addr__shadow = T52;
  IPBlock_pb__out = T10;
  IPBlock_pa__out = T16;
  IPBlock_s__wvalid = T30;
  IPBlock_s__wresp_valid = T35;
  IPBlock_s__rvalid = T49;
  IPBlock_s__addr = T52;
}
