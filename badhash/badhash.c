#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>

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

int main(void) {
  char test[4] = "P35";
  // htonl/ntohl needed to sort byte order problems
  uint32_t hash = ntohl(badhash(htonl(*(uint32_t*)&test)));
  printf("%s\n", (char*)&hash);

  return 0;
}
