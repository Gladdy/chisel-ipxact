```
wire[31:0] x; // input
x = x ^ 32'hD171A769;
res = {
  x[29], x[31:30], x[23:22], x[0], x[6:1], x[17:15], x[12:9], x[28:24],
  x[13:12], x[21:18], x[8:7]
}
```

The string "P35" (0x50333500) should return "OK!" (0x4F4B2100). This is
assuming big-endianness. See badhash.c for a reference C implementation.

The Makefile should work to run the program on the simulator. You can also just
use gcc to build for the host machine. The Makefile needs the path to the
ipyxact checkout in the variable IPYXACT. The default assumes the checkout is a
sibling of the chisel-ipxact checkout.
