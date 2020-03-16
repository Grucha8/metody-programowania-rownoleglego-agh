OUTPUT_FILE_1=wyniki_1_node_sm.txt
OUTPUT_FILE_2=wyniki_1_node_net.txt
OUTPUT_FILE_3=wyniki_2_node_1_phy.txt
OUTPUT_FILE_4=wyniki_2_node_2_phy.txt

all:
	mpicc -std=c99 -o c1.o c1.c
	> $(OUTPUT_FILE_1)
	> $(OUTPUT_FILE_2)
	> $(OUTPUT_FILE_3)
	> $(OUTPUT_FILE_4)
	> S$(OUTPUT_FILE_1)
	> S$(OUTPUT_FILE_2)
	> S$(OUTPUT_FILE_3)
	> S$(OUTPUT_FILE_4)
	for number in 1024 4096 8192 16384 32768 65536 131072 262144 524288 1048576; do \
		mpiexec -machinefile ./case1 -np 2 ./c1.o $$number 0 >> $(OUTPUT_FILE_1) ; \
		mpiexec -machinefile ./case1 -np 2 -env MPIR_CVAR_CH3_NOLOCAL 1 ./c1.o $$number 0 >> $(OUTPUT_FILE_2) ; \
		mpiexec -machinefile ./case3 -np 2 ./c1.o $$number 0 >> $(OUTPUT_FILE_3) ; \
		mpiexec -machinefile ./case4 -np 2 ./c1.o $$number 0 >> $(OUTPUT_FILE_4) ; \
		mpiexec -machinefile ./case1 -np 2 ./c1.o $$number 1 >> S$(OUTPUT_FILE_1) ; \
		mpiexec -machinefile ./case1 -np 2 -env MPIR_CVAR_CH3_NOLOCAL 1 ./c1.o $$number 1 >> S$(OUTPUT_FILE_2) ; \
		mpiexec -machinefile ./case3 -np 2 ./c1.o $$number 1 >> S$(OUTPUT_FILE_3) ; \
		mpiexec -machinefile ./case4 -np 2 ./c1.o $$number 1 >> S$(OUTPUT_FILE_4) ; \
	done