#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define KB 1000
#define MB 1000 * KB

const int NP = 5;
const int array_size = 3 * 5;

const int array_size2 = 5 * 10 * MB;


void first_fun(int world_rank, int* recv_data) {
    int* source_array = (int*) malloc(sizeof(int) * array_size);
    for (int i = 0; i < array_size; i++) {
        source_array[i] = 1;
    }

    MPI_Scatter(source_array, 3, MPI_INT, recv_data, 3, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    for (int i = 0; i < 3; i++) {
        recv_data[i] = recv_data[i] * world_rank;
    }

    int* result_array = (int *) malloc(sizeof(int) * array_size);
    MPI_Gather(recv_data, 3, MPI_INT, result_array, 3, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i =0; i< array_size;i++) {
        printf("%d, ", result_array[i]);
    }
    printf("\n");
}

void fun(int world_rank, int* recv_data) {
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(NULL, 3, MPI_INT, recv_data, 3, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < 3; i++) {
        recv_data[i] = recv_data[i] * world_rank;
    }

    MPI_Gather(recv_data, 3, MPI_INT, NULL, 3, MPI_INT, 0, MPI_COMM_WORLD);
}

void cw1(int world_rank) {
    int* recv_data = (int*) malloc(sizeof(int) * 3);

    if (world_rank == 0) {
        first_fun(world_rank, recv_data);
    } else {
        fun(world_rank, recv_data);
    }
}


// =================================================

void first_fun2(int world_rank, char* recv_data) {
    char* source_array = (char*) malloc(sizeof(char) * array_size2);
    char* result_array = (char *) malloc(sizeof(char) * array_size2);
    for (int i = 0; i < array_size2; i++) {
        source_array[i] = 1;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double t = MPI_Wtime();
    MPI_Scatter(source_array, 10 * MB, MPI_CHAR, recv_data, 10 * MB, MPI_CHAR, 0, MPI_COMM_WORLD);

    MPI_Gather(recv_data, 10 * MB, MPI_CHAR, result_array, 10 * MB, MPI_CHAR, 0, MPI_COMM_WORLD);
    t = MPI_Wtime() - t;

    printf("%d: %f\n", world_rank, t); 
    // printf("ALL: %f\n", all_time);
}

void fun2(int world_rank, char* recv_data) {
    MPI_Barrier(MPI_COMM_WORLD);

    double t = MPI_Wtime();
    MPI_Scatter(NULL, 10 * MB, MPI_CHAR, recv_data, 10 * MB, MPI_CHAR, 0, MPI_COMM_WORLD);

    MPI_Gather(recv_data, 10 * MB, MPI_CHAR, NULL, 10 * MB, MPI_CHAR, 0, MPI_COMM_WORLD);
    t = MPI_Wtime() - t;

    printf("%d: %f\n", world_rank, t); 
}

void cw2(int world_rank) {
    char* recv_data = (char*) malloc(sizeof(char) * 10 * MB);

    if (world_rank == 0) {
        first_fun2(world_rank, recv_data);
    } else {
        fun2(world_rank, recv_data);
    }
}




int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // We are assuming at least 2 processes for this task
    if (world_size < 5) {
        fprintf(stderr, "World size must be greater than 5 for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1); 
    }

    //cw1(world_rank);
    cw2(world_rank);


    MPI_Finalize();

    return 0;
}