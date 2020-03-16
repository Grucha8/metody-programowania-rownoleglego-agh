#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int PING_PONG_LIMIT = 10;
int MSG_SIZE = 1;
int SYNCHRO = 0;
int DELAY_CONUT = 10000;

void do_MPI_Send(char* msg, int msg_size, int partner_rank) {
    if (SYNCHRO) {
        MPI_Ssend(msg, msg_size, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD);        
    } else {
        MPI_Send(msg, msg_size, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD);
    }
}

void calculate_delay(int world_rank) {
    int partner_rank = (world_rank + 1) % 2;
    int count = 0;
    char msg[3] = {'a','b','\0'};
    double t = 0.0;

    if (world_rank == 0) {
        while (count < DELAY_CONUT) {
                MPI_Barrier(MPI_COMM_WORLD);

                t -= MPI_Wtime();
                do_MPI_Send(msg, 3, partner_rank);
                MPI_Recv(msg, 3, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                t += MPI_Wtime();

                count++;
        }
        double delay = t * 1000.0 / DELAY_CONUT * 2.0;
        printf("DELAY: %f\n", delay);

    } else {
        while (count < DELAY_CONUT) {
                MPI_Barrier(MPI_COMM_WORLD);

                MPI_Recv(msg, 3, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                do_MPI_Send(msg, 3, partner_rank);

                count++;
        }
    }
}

float calc_avg(double* elements, int size) {
    double sum = 0;
    int i;
    for (i = 0; i < size; i++) {
        sum += elements[i];
    }
    return sum / size;
}

void fun(int world_rank) {
    int ping_pong_count = 0;
    char* msg = (char *) malloc(sizeof(char) * MSG_SIZE);
    int partner_rank = (world_rank + 1) % 2;
    double* times = (double *) malloc(sizeof(double) * PING_PONG_LIMIT);

    double begin, end, t;

MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 0) {
        printf("%d, ", MSG_SIZE);
        while (ping_pong_count < PING_PONG_LIMIT) {
                //MPI_Barrier(MPI_COMM_WORLD);

                begin = MPI_Wtime();
                do_MPI_Send(msg, MSG_SIZE, partner_rank);
                MPI_Recv(msg, MSG_SIZE, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                end = MPI_Wtime();

                t = (end - begin) / 2;
                times[ping_pong_count] = t;
                ping_pong_count++;
        }
    double avg = calc_avg(times, PING_PONG_LIMIT);
    printf("%f, ", avg);

    int bw = (int) ( ((long long) MSG_SIZE * 8) / (avg * 1000000));
    printf(" %d\n\n", bw);
    } else {
        while (ping_pong_count < PING_PONG_LIMIT) {
                //MPI_Barrier(MPI_COMM_WORLD);

                MPI_Recv(msg, MSG_SIZE, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                do_MPI_Send(msg, MSG_SIZE, partner_rank);

                ping_pong_count++;
        }
    }
}

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1); 
  }
  
  SYNCHRO = atoi(argv[2]);
  MSG_SIZE = atoi(argv[1]);

  calculate_delay(world_rank);
  fun(world_rank);

  MPI_Finalize();

  return 0;
}
