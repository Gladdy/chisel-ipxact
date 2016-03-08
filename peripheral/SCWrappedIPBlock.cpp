#include "IPBlock.h"

// ========================================
// dat_t needs to have this operator defined, so that
// I'm allowed to use it with System C.
// But be warned: I don't quite know what I
// am doing.
// ========================================
template <int w>
inline ostream& operator << (ostream& os, const dat_t<w>& arg){
  return os;
}

struct cs_io_archannel_bits_i {
  dat_t<32> IPBlock__io_archannel_bits_araddr;
  dat_t<3> IPBlock__io_archannel_bits_arprot;
};
inline ostream& operator << (ostream& os, const cs_io_archannel_bits_i& arg){  return os; }

struct cs_io_awchannel_bits_i {
  dat_t<32> IPBlock__io_awchannel_bits_awaddr;
  dat_t<3> IPBlock__io_awchannel_bits_awprot;
};
inline ostream& operator << (ostream& os, const cs_io_awchannel_bits_i& arg){  return os; }

struct cs_io_bchannel_bits_o {
  dat_t<2> IPBlock__io_bchannel_bits_bresp;
};
inline ostream& operator << (ostream& os, const cs_io_bchannel_bits_o& arg){  return os; }

struct cs_io_rchannel_bits_o {
  dat_t<32> IPBlock__io_rchannel_bits_rdata;
  dat_t<2> IPBlock__io_rchannel_bits_rresp;
};
inline ostream& operator << (ostream& os, const cs_io_rchannel_bits_o& arg){  return os; }

struct cs_io_wchannel_bits_i {
  dat_t<32> IPBlock__io_wchannel_bits_wdata;
  dat_t<4> IPBlock__io_wchannel_bits_wstrb;
};
inline ostream& operator << (ostream& os, const cs_io_wchannel_bits_i& arg){  return os; }


SC_MODULE(SCWrappedIPBlock){
  IPBlock_t* c;
  
  // sc_fifo<dat_t<1> >* in;
  sc_fifo<cs_io_archannel_bits_i >* archannel;
  sc_fifo<cs_io_awchannel_bits_i >* awchannel;
  sc_fifo<cs_io_wchannel_bits_i >* wchannel;
  
  
  // sc_fifo<dat_t<1> >* out;
  sc_fifo<cs_io_bchannel_bits_o >* bchannel;
  sc_fifo<cs_io_rchannel_bits_o >* rchannel;
  

  SC_HAS_PROCESS(SCWrappedIPBlock);
  SCWrappedIPBlock(sc_module_name a_name) : sc_module(a_name) {
    // Initialize Component, Clock in RESET
    c = new IPBlock_t();
    c->init();

    // Initialize Output Fifos
    // out = new sc_fifo<dat_t<1> >(1);
    bchannel = new sc_fifo<cs_io_bchannel_bits_o >(1);
  rchannel = new sc_fifo<cs_io_rchannel_bits_o >(1);
  

    // Clock Initialization?
    // Don't understand what this is for. Copied from emulator.
    for(int i = 0; i < 5; i++) {
      dat_t<1> reset = LIT<1>(1);
      c->clock_lo(reset);
      c->clock_hi(reset);
    }

    //Register Thread
    SC_THREAD(component_thread);
  }

  void component_thread(void){
    //Buffer for input data
    // dat_t<1> in_data;
    // int has_in = 0;
    cs_io_archannel_bits_i dat0;
    int filled1 = 0;
    cs_io_awchannel_bits_i dat2;
    int filled3 = 0;
    cs_io_wchannel_bits_i dat4;
    int filled5 = 0;
    

    while(true){
      //Is there input waiting?
      // if(!has_in){has_in = in->nb_read(in_data);}
      // c->GCD__io_a = in_data;
      // c->GCD__io_v1 = LIT<1>(has_in);
      if(!filled1){filled1 = archannel->nb_read(dat0);}
      c->IPBlock__io_archannel_bits_araddr = dat0.IPBlock__io_archannel_bits_araddr;
      c->IPBlock__io_archannel_bits_arprot = dat0.IPBlock__io_archannel_bits_arprot;
      c->IPBlock__io_archannel_valid = LIT<1>(filled1);
      if(!filled3){filled3 = awchannel->nb_read(dat2);}
      c->IPBlock__io_awchannel_bits_awaddr = dat2.IPBlock__io_awchannel_bits_awaddr;
      c->IPBlock__io_awchannel_bits_awprot = dat2.IPBlock__io_awchannel_bits_awprot;
      c->IPBlock__io_awchannel_valid = LIT<1>(filled3);
      if(!filled5){filled5 = wchannel->nb_read(dat4);}
      c->IPBlock__io_wchannel_bits_wdata = dat4.IPBlock__io_wchannel_bits_wdata;
      c->IPBlock__io_wchannel_bits_wstrb = dat4.IPBlock__io_wchannel_bits_wstrb;
      c->IPBlock__io_wchannel_valid = LIT<1>(filled5);
      

      //Are output queues ready?      
      // c->GCD__io_r2 = LIT<1>(out->num_free() > 0);
      c->IPBlock__io_bchannel_ready = LIT<1>(bchannel->num_free() > 0);
      c->IPBlock__io_rchannel_ready = LIT<1>(rchannel->num_free() > 0);
            

      //Clock Lo
      dat_t<1> reset = LIT<1>(0);
      c->clock_lo(reset);

      //Has input been accepted?
      // if(c->GCD__io_r1.values[0]) has_in = 0;
      if(c->IPBlock__io_archannel_ready.values[0]) filled1 = 0;
      if(c->IPBlock__io_awchannel_ready.values[0]) filled3 = 0;
      if(c->IPBlock__io_wchannel_ready.values[0]) filled5 = 0;
      
      
      //Is output ready?
      // if(c->GCD__io_v2.values[0]) out->nb_write(c->GCD__io_z);
      if(c->IPBlock__io_bchannel_valid.values[0]) {
        cs_io_bchannel_bits_o dato;
        dato.IPBlock__io_bchannel_bits_bresp = c->IPBlock__io_bchannel_bits_bresp;
        bchannel->nb_write(dato);
      }
    if(c->IPBlock__io_rchannel_valid.values[0]) {
        cs_io_rchannel_bits_o dato;
        dato.IPBlock__io_rchannel_bits_rdata = c->IPBlock__io_rchannel_bits_rdata;
        dato.IPBlock__io_rchannel_bits_rresp = c->IPBlock__io_rchannel_bits_rresp;
        rchannel->nb_write(dato);
      }
    

      //Clock Hi
      wait(1.0, SC_SEC);
      c->clock_hi(reset);
    }
  }
};
