#include "HelloModule.h"

int main (int argc, char* argv[]) {
  HelloModule_t module;
  HelloModule_api_t api(&module);
  module.init();
  api.init_sim_data();
  FILE *f = NULL;
  module.set_dumpfile(f);
  module.clk.cnt = module.clk.len;
  while(!api.exit()) api.tick();
  if (f) fclose(f);
}
