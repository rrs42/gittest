
all : test_h 
.PHONY: all

UNAME=$(shell uname)

ifeq ($(UNAME),Darwin)
  binfmt=macho64
  extralibs=
else
  binfmt=elf64
  extralibs= -lrt
endif

clean : 
	rm -f asmtest test_h *.o *.lst
.PHONY: clean

%.o : %.asm
	nasm -f $(binfmt) -D UNAME=$(UNAME) -g -l $(basename $<).lst -o $@ $<

asmtest : asmtest.o
	ld -o $@ $^

asmtest.o : asmtest.asm


test_h : test_h.o to_dec.o to_dec_c.o
	g++ -g -o test_h test_h.o to_dec.o to_dec_c.o $(extralibs)

to_dec.o : to_dec.asm

to_dec_c.o : to_dec_c.c
	g++ -g -O2 -c -o to_dec_c.o to_dec_c.c

test_h.o : test_h.c
	g++ -g -c -o test_h.o test_h.c


