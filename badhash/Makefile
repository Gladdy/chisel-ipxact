# Copyright (C) 2015 XPARCH, Ltd. <info@xparch.com>
#
# Hello World - most simple program possible.   If using just writes it should compile without even libc and should either use a putchar ISS backdoor or else write to the uart using craft_wrch.
#



all:badhash-clr


# Generally, set this in the environment not here please.  But for badhash world we want to keep it all simple.
USE_THUMB=1
USE_PRLIBC=1

include $(PRAZOR)/vhls/images/Makefile.inc

IPYXACT?=../../ipyxact

CC=arm-linux-gnueabihf-gcc
AS=arm-linux-gnueabihf-as
LD=arm-linux-gnueabihf-ld
OBJD=arm-linux-gnueabihf-objdump

CFLAGS += -DUNICORE
LDFLAGS += -L/home/ams247/tools/arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.3

EXTRALIBS += 

# one core command line
CMD=LD_LIBRARY_PATH=/usr/local/lib  $(SIM) $(DRAM_FLAGS) -cores 1 -tracelevel 0 -global-qk-ns 1 -no-caches 
#CMD=$(SIM) $(DRAM_FLAGS) -self-starting-cores 2 -no-caches -global-qk-ns 100 -vcd-power-plot -tracelevel 1 -watch b8 +20 -tracelevel 0

badhash.h: ../badhash.xml
	python $(IPYXACT)/gen_c_header.py $< -o $@

badhash.o: badhash.c badhash.h
	$(CC)  $(CFLAGS) -I$(DDRIVERS)/uart64_cbg -c $<

badhash-clr:  badhash run

badhash: badhash.o $(LIBS) $(CRT1) $(EXTRALIBS) prlibc.o
	$(LD)  $(LDFLAGS) -o $@ $(CRT1) $< $(EXTRALIBS)  $(LIBS) $(LDLIBS) 
	$(OBJD) -d $@ > badhash.dis

run:
	ulimit -t 100 ; $(CMD) -image ./badhash -name nominal  -- red yellow green blue well done milos 
#	ulimit -t 10 ; $(CMD) -image ./badhash -name no-caches -no-caches  -- red yellow green blue well done milos 
	echo done

clean:
	rm -f badhash badhash.h *.s *.o *.dis *.txt


# eof
