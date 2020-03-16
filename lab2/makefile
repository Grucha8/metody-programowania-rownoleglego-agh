all:
	mpicc -std=c99 hw.c -o hw.o
	mpiexec -machinefile ./allnodes -np 5 ./hw.o