#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int ELEMENTS_NUMBER = 5;

float* generate_random_numbers(int number_of_elements) {
    float* elements = (float*) malloc(sizeof(float) * number_of_elements);

    for (int i = 0; i < number_of_elements; i++) {
        elements[i] = (rand() / (float)RAND_MAX);
    }
    
    return elements;
}

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    srand(time(NULL) * world_rank);
    float* rand_nums = generate_random_numbers(ELEMENTS_NUMBER);

    // Sum the numbers locally
    float local_sum = 0;
    int i;
    for (i = 0; i < ELEMENTS_NUMBER; i++) {
        local_sum += rand_nums[i];
    }

    // Print the random numbers on each process
    printf("Local sum for process %d - %f, avg = %f\n",
        world_rank, local_sum, local_sum / ELEMENTS_NUMBER);

    MPI_Barrier(MPI_COMM_WORLD);
    // Reduce all of the local sums into the global sum
    float global_sum;
    double t = MPI_Wtime();
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_PROD, 0, MPI_COMM_WORLD);
    t = MPI_Wtime() - t;

    // Print the result
    printf("Time for %d : %f\n", world_rank, t);
    if (world_rank == 0) {
        printf("Total sum = %f\n", global_sum);
    }


    free(rand_nums);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}