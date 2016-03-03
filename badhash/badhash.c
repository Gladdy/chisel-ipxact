#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>

// https://github.com/andrewscull/ipyxact
// python gen_c_header.py badhash.xml -o badhash.h
#include "badhash.h"

uint32_t badhash(uint32_t x) {
  x ^= 0xD171A769;
  return
    ((x << 26) & 0x04000000) |
    ((x << 19) & 0x03F00000) |
    ((x <<  5) & 0x18000000) |
    ((x <<  4) & 0x0001E000) |
    ((x <<  2) & 0x800E0000) |
    ((x >> 16) & 0x00001F3C) |
    ((x >>  7) & 0x000000C3) |
    ((x >>  1) & 0x60000000);
}

int main(int argc, char** argv) {
  // htonl/ntohl needed to sort byte order problems
  char test[4] = "P35";
  uint32_t hash;

  // Run it in software
  hash = ntohl(badhash(htonl(*(uint32_t*)&test)));
  printf("Software says: %s\n", (char*)&hash);

  // Run it on the hardware
  BADHASH_REGISTER_MAP_INPUT_REG = htonl(*(uint32_t*)&test);
  hash = ntohl(BADHASH_REGISTER_MAP_OUTPUT_REG);
  printf("Hardware says: %s\n", (char*)&hash);

  return 0;
}
