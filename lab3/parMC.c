#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

long double R_PI = 3.14159265358979323L;

long long int count_points_in_circle(long long int number_of_points) {
    long long int i, circle_points = 0;
    double rand_x, rand_y, origin_dist;

    for (i = 0; i < number_of_points; i++) { 
        rand_x = (double) (rand() / ((double) RAND_MAX)); 
        rand_y = (double) (rand() / ((double) RAND_MAX)); 

        origin_dist = rand_x * rand_x + rand_y * rand_y; 
  
        if (origin_dist <= 1) 
            circle_points++; 
    }

    return circle_points;
}

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (argc != 2) {
        return 1;
    }
    long long int number_of_points = strtoll(argv[1], NULL, 0);

    srand(time(NULL) * world_rank);

    MPI_Barrier(MPI_COMM_WORLD);
    double t = MPI_Wtime();
    long long int points_in_circle = count_points_in_circle(number_of_points / world_size);
    long long int all_points_in_circle;

    MPI_Reduce(&points_in_circle, &all_points_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    t = MPI_Wtime() - t;

    if (world_rank == 0) {
        long double pi = ((long double) 4 * all_points_in_circle) / ((long double) number_of_points);
        printf("=== AND FOR %i PROCESSORS ===\n", world_size);
        printf("PI: %Lf\n", pi);
        printf("DIF: %.14f\n", fabs(R_PI - pi));
        printf("TIME: %f\n\n", t);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}