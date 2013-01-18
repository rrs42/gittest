
all : asmtest test_h 
.PHONY: all


clean : 
	rm asmtest test_h *.o *.lst
.PHONY: clean

%.o : %.asm
	nasm -f elf64 -g -l $(basename $<).lst -o $@ $<

asmtest : asmtest.o
	ld -o $@ $^

asmtest.o : asmtest.asm


test_h : test_h.o to_dec.o to_dec_c.o
	g++ -g -o test_h test_h.o to_dec.o to_dec_c.o -lrt

to_dec.o : to_dec.asm

to_dec_c.o : to_dec_c.c
	g++ -g -O -c -o to_dec_c.o to_dec_c.c

test_h.o : test_h.c
	g++ -g -c -o test_h.o test_h.c


