#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


long double PI = 3.14159265358979323L;

double calc_pi(long long int number_of_points) {
    long long int i, circle_points = 0;
    double rand_x, rand_y, origin_dist;

    for (i = 0; i < number_of_points; i++) { 
        rand_x = (double) (rand() / ((double) RAND_MAX)); 
        rand_y = (double) (rand() / ((double) RAND_MAX)); 

        origin_dist = rand_x * rand_x + rand_y * rand_y; 
  
        if (origin_dist <= 1) 
            circle_points++; 
    }

    return (double) (4 * circle_points) / (double) number_of_points;
}

int main(int argc, char** argv) {
    long long int number_of_points; 
    int calc_repeat;

    if (argc != 3) {
        return 1;
    }
    number_of_points = strtoll(argv[1], NULL, 0);
    calc_repeat = atoi(argv[2]);

    srand(time(NULL));

    double avg = 0.0, std_dev = 0.0, avg_t = 0.0;
    double* pi_results = (double*) malloc(sizeof(double) * calc_repeat);
     for (int i = 0; i < calc_repeat; i++) {
        clock_t t = clock();
        pi_results[i] = calc_pi(number_of_points);
        avg_t += clock() - t;
        avg += pi_results[i];        
    }
    avg /= calc_repeat;
    avg_t = avg_t / (CLOCKS_PER_SEC * calc_repeat); 
        
    for (int i = 0; i < calc_repeat; i++) {
        std_dev += (pi_results[i] - avg) * (pi_results[i] - avg);
    }
    std_dev = sqrt(std_dev / calc_repeat);
    
    printf("=== RESULT FOR %d POINTS ===\n", number_of_points);
    printf("AVG: %f\n", avg);
    printf("DEV: %f\n", std_dev);
    printf("DIF: %lf\n", fabs(PI - avg));
    printf("TIME: %f\n\n", avg_t);

    return 0;
}