
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


test_h : test_h.o to_dec.o
	g++ -g -o test_h test_h.o to_dec.o -lrt

to_dec.o : to_dec.asm

test_h.o : test_h.c
	g++ -g -c -o test_h.o test_h.c


