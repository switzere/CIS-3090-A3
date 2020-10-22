
LIB = -L/usr/lib/x86_64-linux-gnu/openmpi/lib
INCLUDE = -I/usr/lib/x86_64-linux-gnu/openmpi/include

all: clean mpish

clean:
	rm mpish

mpish:
	gcc mpish.c -o mpish -lmpi   $(LIB) $(INCLUDE) -lm

rmdir:
	rmdir p1
	rmdir p2
	rmdir p3
	rmdir p4
	rmdir p5
