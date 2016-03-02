#include "HelloModule.h"

void HelloModule_t::init ( val_t rand_init ) {
  this->__srand(rand_init);
  clk.len = 1;
  clk.cnt = 0;
  clk.values[0] = 0;
}


int HelloModule_t::clock ( dat_t<1> reset ) {
  uint32_t min = ((uint32_t)1<<31)-1;
  if (clk.cnt < min) min = clk.cnt;
  clk.cnt-=min;
  if (clk.cnt == 0) clock_lo( reset );
  if (clk.cnt == 0) clock_hi( reset );
  if (clk.cnt == 0) clk.cnt = clk.len;
  return min;
}


void HelloModule_t::print ( FILE* f ) {
#if __cplusplus >= 201103L
  if (T0.values[0]) dat_fprintf<440>(f, "Hello Worldaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa!\n");
#endif
fflush(f);
}
void HelloModule_t::print ( std::ostream& s ) {
#if __cplusplus >= 201103L
  if (T0.values[0]) dat_prints<440>(s, "Hello Worldaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa!\n");
#endif
s.flush();
}


void HelloModule_t::dump_init ( FILE* f ) {
}


void HelloModule_t::dump ( FILE* f, int t, dat_t<1> reset ) {
}




void HelloModule_t::clock_lo ( dat_t<1> reset, bool assert_fire ) {
  { T0.values[0] = reset.values[0] ^ 0x1L;}
}


void HelloModule_t::clock_hi ( dat_t<1> reset ) {
}


void HelloModule_api_t::init_sim_data (  ) {
  sim_data.inputs.clear();
  sim_data.outputs.clear();
  sim_data.signals.clear();
  HelloModule_t* mod = dynamic_cast<HelloModule_t*>(module);
  assert(mod);
  sim_data.clk_map["clk"] = new clk_api(&mod->clk);
}
