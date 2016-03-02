#include "Hello.h"

void Hello_t::init ( val_t rand_init ) {
  this->__srand(rand_init);
  clk.len = 1;
  clk.cnt = 0;
  clk.values[0] = 0;
}


int Hello_t::clock ( dat_t<1> reset ) {
  uint32_t min = ((uint32_t)1<<31)-1;
  if (clk.cnt < min) min = clk.cnt;
  clk.cnt-=min;
  if (clk.cnt == 0) clock_lo( reset );
  mod_t::dump( reset );
  if (clk.cnt == 0) clock_hi( reset );
  if (clk.cnt == 0) clk.cnt = clk.len;
  return min;
}


void Hello_t::print ( FILE* f ) {
}
void Hello_t::print ( std::ostream& s ) {
}


void Hello_t::dump_init ( FILE* f ) {
  fputs("$timescale 1ps $end\n", f);
  fputs("$scope module Hello $end\n", f);
  fputs("$var wire 1 \x21 clk $end\n", f);
  fputs("$var wire 1 \x22 reset $end\n", f);
  fputs("$var wire 8 \x23 io_out $end\n", f);
  fputs("$upscope $end\n", f);
  fputs("$enddefinitions $end\n", f);
  fputs("$dumpvars\n", f);
  fputs("$end\n", f);
  fputs("#0\n", f);
  if (clk.cnt == 0) {
    clk.values[0] = 1;
    dat_dump<1>(f, clk, 0x21);
  }
  dat_t<1> reset = LIT<1>(1);
  dat_dump<1>(f, reset, 0x22);
  dat_dump<1>(f, Hello__io_out, 0x23);
  Hello__io_out__prev = Hello__io_out;
  fputs("#1\n", f);
  if (clk.cnt == 0) {
    clk.values[0] = 0;
    dat_dump<1>(f, clk, 0x21);
  }
}


void Hello_t::dump ( FILE* f, int t, dat_t<1> reset ) {
  if (t == 0) return dump_init(f);
  fprintf(f, "#%d\n", t << 1);
  if (clk.cnt == 0)  goto L0;
K0:  if (reset != reset__prev)  goto L1;
K1:  if (Hello__io_out != Hello__io_out__prev)  goto L2;
K2:  fprintf(f, "#%d\n", (t << 1) + 1);
  if (clk.cnt == 0)  goto Z0;
C0:  return;
L0:
  clk.values[0] = 1;
  dat_dump<1>(f, clk, 0x21);
  goto K0;
L1:
  reset__prev = reset;
  dat_dump<1>(f, reset, 0x22);
  goto K1;
L2:
  Hello__io_out__prev = Hello__io_out;
  dat_dump<1>(f, Hello__io_out, 0x23);
  goto K2;
Z0:
  clk.values[0] = 0;
  dat_dump<1>(f, clk, 0x21);
  goto C0;
}




void Hello_t::clock_lo ( dat_t<1> reset, bool assert_fire ) {
  { Hello__io_out.values[0] = 0x2aL;}
}


void Hello_t::clock_hi ( dat_t<1> reset ) {
}


void Hello_api_t::init_sim_data (  ) {
  sim_data.inputs.clear();
  sim_data.outputs.clear();
  sim_data.signals.clear();
  Hello_t* mod = dynamic_cast<Hello_t*>(module);
  assert(mod);
  sim_data.outputs.push_back(new dat_api<8>(&mod->Hello__io_out));
  sim_data.clk_map["clk"] = new clk_api(&mod->clk);
}
