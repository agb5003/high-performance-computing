/* High Performance Computing
    Question 2, November 10 2026
    Maximilian Fernaldy
    C6IM1507
*/

#include <stdio.h> // printf
#include <stdlib.h> // srand
#include <time.h> // time

int main(int argc, char* argv[])
{
    struct timespec start, end;

    int N = 100000;
    int i, total = 0;
    double x, y;

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < N; i++) {
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;
        if (x*x + y*y < 1) {
            total++;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (double)(end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);

    printf("pi = %lf\n", 4.0*total/N);
    printf("elapsed time: %lf [sec]\n", elapsed);
}