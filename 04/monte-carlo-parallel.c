/* High Performance Computing
    Question 2, November 10 2026
    Maximilian Fernaldy
    C6IM1507
*/

#include <stdio.h> // printf
#include <stdlib.h> // srand
#include <time.h> // time

#include <mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int id, p;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int N = 100000;
    int local_n = N/p + (id < N % p); // Add 1 if id is lesser than the N%p
    // For example, if N = 25 and p = 7, 25%7 = 4, so four ranks/ids from 0-3 will have 25/7+1=4
    // trials instead of 3 to keep the total # of trials totaling 25 instead of 21 (if the "leap"
    // trials were not included).
    int local_total = 0;
    int total = 0;
    double x, y;

    // Use pid 0 to seed rng and broadcast to other processes
    unsigned base_seed;
    if (id == 0)
        base_seed = time(NULL);
    
    MPI_Bcast(&base_seed, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    srand(base_seed + id);

    MPI_Barrier(MPI_COMM_WORLD);
    double etime = -MPI_Wtime();

    for (int i = 0; i < local_n; i++) {
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;
        if (x*x + y*y < 1) {
            local_total++;
        }
    }

    MPI_Reduce(&local_total, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    etime += MPI_Wtime();

    if (id == 0) {
        // Only print results with 1 process, otherwise output will be replicated many times
        printf("pi = %lf\n", 4.0*total/N);
        printf("elapsed time: %lf [sec]\n", etime);
    }

    MPI_Finalize();
}