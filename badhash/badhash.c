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

uint32_t bswap32(uint32_t x) {
 return  ((x << 24) & 0xff000000 ) |
     ((x <<  8) & 0x00ff0000 ) |
     ((x >>  8) & 0x0000ff00 ) |
     ((x >> 24) & 0x000000ff );
}

// Can't get the library to work on the simuator so implement them here.
uint64_t to_big_endian_64(uint64_t x)
{
#if BYTE_ORDER == LITTLE_ENDIAN
  return (uint64_t)bswap32(x)
    | ((uint64_t)bswap32((x >> 32) & 0xFFFFFFFF) << 32);
#else
  return x;
#endif
}
uint64_t from_big_endian_64(uint64_t x)
{
  return to_big_endian_64(x);
}

uint32_t badhash_inner(uint32_t x) {
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

uint64_t badhash(uint64_t x) {
  uint64_t res = badhash_inner((x >> 32) & 0xFFFFFFFF);
  return (uint64_t)badhash_inner(x) | (res << 32);
}

int main(int argc, char** argv) {
  uint64_t hash;
  // 0x5033350050333500ULL == "P35\0P35""

  printf("\n\n\n\n\n\n");

  // Run it in software
  hash = from_big_endian_64(badhash(0x5033350050333500ULL));
  printf("Software says: %s%s\n", (char*)&hash, ((char*)&hash)+4);

  // Run it on the hardware
  BADHASH_REGS_INPUT_DATA = 0x5033350050333500ULL;
  BADHASH_REGS_INPUT_READY = 1;
  while (!BADHASH_REGS_OUTPUT_READY);
  hash = from_big_endian_64(BADHASH_REGS_OUTPUT_DATA);
  const char* res = hash == 0x4F4B21004F4B2100ULL ? (char*)&hash : "TEST FAILED!";
  printf("Hardware says: %s%s\n", res, res+4);

  printf("\n\n\n\n\n\n");

#ifdef SIMULATOR
  SOCDAM_KILL_SIM(0);
#endif
  return 0;
}

// Do nothing. Does it need to?
void _isr_routine(void) {}
