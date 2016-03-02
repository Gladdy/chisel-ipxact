#include "BadHash.h"

void BadHash_t::init ( val_t rand_init ) {
  this->__srand(rand_init);
  BadHash__operating_register.randomize(&__rand_seed);
  BadHash__running.randomize(&__rand_seed);
  clk.len = 1;
  clk.cnt = 0;
  clk.values[0] = 0;
}


int BadHash_t::clock ( dat_t<1> reset ) {
  uint32_t min = ((uint32_t)1<<31)-1;
  if (clk.cnt < min) min = clk.cnt;
  clk.cnt-=min;
  if (clk.cnt == 0) clock_lo( reset );
  if (clk.cnt == 0) clock_hi( reset );
  if (clk.cnt == 0) clk.cnt = clk.len;
  return min;
}


void BadHash_t::print ( FILE* f ) {
}
void BadHash_t::print ( std::ostream& s ) {
}


void BadHash_t::dump_init ( FILE* f ) {
}


void BadHash_t::dump ( FILE* f, int t, dat_t<1> reset ) {
}




void BadHash_t::clock_lo ( dat_t<1> reset, bool assert_fire ) {
  val_t T0;
  T0 = BadHash__running.values[0] == 0x0L;
  val_t T1;
  T1 = BadHash__io_in_ready.values[0] == 0x1L;
  val_t T2;
  { T2 = T1 & T0;}
  val_t T3;
  { T3 = TERNARY_1(T2, BadHash__io_in_data.values[0], BadHash__operating_register.values[0]);}
  { T4.values[0] = TERNARY(reset.values[0], 0x0L, T3);}
  val_t T5;
  { T5 = BadHash__operating_register.values[0] ^ 0x1ea14969L;}
  val_t T6;
  T6 = BadHash__running.values[0] == 0x1L;
  val_t T7;
  { T7 = T2 ^ 0x1L;}
  val_t T8;
  { T8 = T7 & T6;}
  val_t T9;
  { T9 = TERNARY(T8, T5, 0x0L);}
  { BadHash__io_out_data.values[0] = T9;}
  val_t T10;
  { T10 = TERNARY(T2, 0x1L, BadHash__running.values[0]);}
  val_t T11;
  { T11 = TERNARY(T8, 0x0L, T10);}
  { T12.values[0] = TERNARY(reset.values[0], 0x0L, T11);}
  val_t T13;
  { T13 = T8 | 0x0L << 1;}
  { BadHash__io_out_ready.values[0] = T13;}
}


void BadHash_t::clock_hi ( dat_t<1> reset ) {
  dat_t<32> BadHash__operating_register__shadow = T4;
  dat_t<1> BadHash__running__shadow = T12;
  BadHash__operating_register = T4;
  BadHash__running = T12;
}


void BadHash_api_t::init_sim_data (  ) {
  sim_data.inputs.clear();
  sim_data.outputs.clear();
  sim_data.signals.clear();
  BadHash_t* mod = dynamic_cast<BadHash_t*>(module);
  assert(mod);
  sim_data.inputs.push_back(new dat_api<32>(&mod->BadHash__io_in_data));
  sim_data.inputs.push_back(new dat_api<1>(&mod->BadHash__io_in_ready));
  sim_data.outputs.push_back(new dat_api<32>(&mod->BadHash__io_out_data));
  sim_data.outputs.push_back(new dat_api<32>(&mod->BadHash__io_out_ready));
  sim_data.signals.push_back(new dat_api<32>(&mod->BadHash__operating_register));
  sim_data.signal_map["BadHash.operating_register"] = 0;
  sim_data.signals.push_back(new dat_api<1>(&mod->BadHash__running));
  sim_data.signal_map["BadHash.running"] = 1;
  sim_data.clk_map["clk"] = new clk_api(&mod->clk);
}
