#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    long long int number_of_points, i; 
    double rand_x, rand_y, origin_dist, pi; 
    long long int circle_points = 0;

    number_of_points = strtoll(argv[1], NULL, 0);

    srand(time(NULL));


    for (i = 0; i < number_of_points; i++) { 

        // Randomly generated x and y values 
        rand_x = (double) (rand() / ((double) RAND_MAX)); 
        rand_y = (double) (rand() / ((double) RAND_MAX)); 

        origin_dist = rand_x * rand_x + rand_y * rand_y; 
  
        if (origin_dist <= 1) 
            circle_points++; 
    }
    pi = (double) (4 * circle_points) / (double) number_of_points;

    printf("PI: 3.1415926535897932384\n");
    printf("PI: %f\n", pi);

    return 0;
}