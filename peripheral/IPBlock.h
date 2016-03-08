#ifndef __IPBlock__
#define __IPBlock__

#include "emulator.h"

class IPBlock_t : public mod_t {
 private:
  val_t __rand_seed;
  void __srand(val_t seed) { __rand_seed = seed; }
  val_t __rand_val() { return ::__rand_val(&__rand_seed); }
 public:
  dat_t<1> IPBlock__io_wchannel_valid;
  dat_t<1> IPBlock_s__io_axi_wchannel_valid;
  dat_t<1> IPBlock_s__io_slaveB_in_valid;
  dat_t<1> IPBlock_pb__io_in_valid;
  dat_t<1> reset;
  dat_t<1> IPBlock_pb__reset;
  dat_t<1> IPBlock_s__io_slaveA_in_valid;
  dat_t<1> IPBlock_pa__io_in_valid;
  dat_t<1> IPBlock_pa__reset;
  dat_t<1> IPBlock_s__io_axi_awchannel_ready;
  dat_t<1> IPBlock__io_awchannel_valid;
  dat_t<1> IPBlock_s__io_axi_awchannel_valid;
  dat_t<1> IPBlock_pa__io_in_ready;
  dat_t<1> IPBlock_s__io_slaveA_in_ready;
  dat_t<1> IPBlock_pb__io_in_ready;
  dat_t<1> IPBlock_s__io_slaveB_in_ready;
  dat_t<1> IPBlock_s__io_axi_wchannel_ready;
  dat_t<1> IPBlock_s__reset;
  dat_t<1> T30;
  dat_t<1> IPBlock_s__wvalid;
  dat_t<1> IPBlock__io_bchannel_ready;
  dat_t<1> IPBlock_s__io_axi_bchannel_ready;
  dat_t<1> IPBlock_s__io_axi_bchannel_valid;
  dat_t<1> T35;
  dat_t<1> IPBlock_s__wresp_valid;
  dat_t<1> IPBlock_s__io_axi_archannel_ready;
  dat_t<1> IPBlock__io_archannel_valid;
  dat_t<1> IPBlock_s__io_axi_archannel_valid;
  dat_t<1> IPBlock__io_rchannel_ready;
  dat_t<1> IPBlock_s__io_axi_rchannel_ready;
  dat_t<1> IPBlock_pa__io_out_valid;
  dat_t<1> IPBlock_s__io_slaveA_out_valid;
  dat_t<1> IPBlock_pb__io_out_valid;
  dat_t<1> IPBlock_s__io_slaveB_out_valid;
  dat_t<1> IPBlock_s__io_axi_rchannel_valid;
  dat_t<1> T49;
  dat_t<1> IPBlock_s__rvalid;
  dat_t<1> IPBlock__io_awchannel_ready;
  dat_t<1> IPBlock__io_wchannel_ready;
  dat_t<1> IPBlock__io_bchannel_valid;
  dat_t<1> IPBlock__io_archannel_ready;
  dat_t<1> IPBlock__io_rchannel_valid;
  dat_t<2> IPBlock_s__io_axi_rchannel_bits_rresp;
  dat_t<2> IPBlock_s__io_axi_bchannel_bits_bresp;
  dat_t<2> IPBlock__io_bchannel_bits_bresp;
  dat_t<2> IPBlock__io_rchannel_bits_rresp;
  dat_t<3> IPBlock__io_archannel_bits_arprot;
  dat_t<3> IPBlock__io_awchannel_bits_awprot;
  dat_t<4> IPBlock__io_wchannel_bits_wstrb;
  dat_t<32> IPBlock__io_wchannel_bits_wdata;
  dat_t<32> IPBlock_s__io_axi_wchannel_bits_wdata;
  dat_t<32> IPBlock_s__io_slaveB_in_bits_wdata;
  dat_t<32> IPBlock_pb__io_in_bits_wdata;
  dat_t<32> T10;
  dat_t<32> IPBlock_pb__out;
  dat_t<32> IPBlock_s__io_slaveA_in_bits_wdata;
  dat_t<32> IPBlock_pa__io_in_bits_wdata;
  dat_t<32> T16;
  dat_t<32> IPBlock_pa__out;
  dat_t<32> IPBlock__io_archannel_bits_araddr;
  dat_t<32> IPBlock_s__io_axi_archannel_bits_araddr;
  dat_t<32> IPBlock__io_awchannel_bits_awaddr;
  dat_t<32> IPBlock_s__io_axi_awchannel_bits_awaddr;
  dat_t<32> T52;
  dat_t<32> IPBlock_s__addr;
  dat_t<32> IPBlock_pa__io_out_bits_rdata;
  dat_t<32> IPBlock_s__io_slaveA_out_bits_rdata;
  dat_t<32> IPBlock_pb__io_out_bits_rdata;
  dat_t<32> IPBlock_s__io_slaveB_out_bits_rdata;
  dat_t<32> IPBlock_s__io_axi_rchannel_bits_rdata;
  dat_t<32> IPBlock__io_rchannel_bits_rdata;
  clk_t clk;

  void init ( val_t rand_init = 0 );
  void clock_lo ( dat_t<1> reset, bool assert_fire=true );
  void clock_hi ( dat_t<1> reset );
  int clock ( dat_t<1> reset );
  void print ( FILE* f );
  void print ( std::ostream& s );
  void dump ( FILE* f, int t, dat_t<1> reset=LIT<1>(0) );
  void dump_init ( FILE* f );

};



#endif
