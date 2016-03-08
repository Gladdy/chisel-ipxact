#ifdef USE_PRLIBC
#define SIMULATOR
#include "prstdio.h"
#else
#include <stdio.h>
#endif
#include <stdint.h>

// https://github.com/andrewscull/ipyxact
// python gen_c_header.py badhash.xml -o badhash.h
#include "badhash.h"

#ifdef SIMULATOR
#include "socdam_bdoor_mapping.h"
#endif

// Can't get the library to work on the simuator so implement them here.
uint32_t htonl(uint32_t x)
{
#if BYTE_ORDER == LITTLE_ENDIAN
  uint8_t* s = (uint8_t*)&x;
  return (uint32_t)(s[0] << 24 | s[1] << 16 | s[2] << 8 | s[3]);
#else
  return x;
#endif
}
uint32_t ntohl(uint32_t x)
{
  return htonl(x);
}

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

  printf("\n\n\n\n\n\n");

  // Run it in software
  hash = ntohl(badhash(htonl(*(uint32_t*)&test)));
  printf("Software says: %s\n", (char*)&hash);

  // Run it on the hardware
  BADHASH_REGS_BUFFER = htonl(*(uint32_t*)&test);
  BADHASH_REGS_IN_READY = 1;
  while (!BADHASH_REGS_OUT_READY);
  hash = ntohl(BADHASH_REGS_HASHER);
  const char* res = hash == 0x4F4B2100 ? (char*)&hash : "TEST FAILED!";
  printf("Hardware says: %s\n", res);

  printf("\n\n\n\n\n\n");

#ifdef SIMULATOR
  SOCDAM_KILL_SIM(0);
#endif
  return 0;
}

// Do nothing. Does it need to?
void _isr_routine(void) {}
