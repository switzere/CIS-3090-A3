
LIB = -L/usr/lib/x86_64-linux-gnu/openmpi/lib
INCLUDE = -I/usr/lib/x86_64-linux-gnu/openmpi/include

all: mpi_hello mpi_SendRec mpi_Bcast

mpi_hello:
	gcc mpi_hello.c -o mpi_hello -lmpi   $(LIB) $(INCLUDE)

mpi_SendRec:
	gcc mpi_SendRec.c -o mpi_SendRec -lmpi   $(LIB) $(INCLUDE)

mpi_Bcast:
	gcc mpi_Bcast.c -o mpi_Bcast -lmpi   $(LIB) $(INCLUDE)

