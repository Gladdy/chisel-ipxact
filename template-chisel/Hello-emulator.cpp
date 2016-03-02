#include "Hello.h"

int main (int argc, char* argv[]) {
  Hello_t module;
  Hello_api_t api(&module);
  module.init();
  api.init_sim_data();
  FILE *f = fopen("./Hello.vcd", "w");
  module.set_dumpfile(f);
  module.clk.cnt = module.clk.len;
  while(!api.exit()) api.tick();
  if (f) fclose(f);
}
