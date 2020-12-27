#include <omp.h>
#define NUM_THREADS 2
void main()
{
    int A[NUM_THREADS], B[NUM_THREADS];
    #pragma omp parallel 
    {    omp_set_num_threads(NUM_THREADS);
        int id = omp_get_num_threads();
        A[id] = big_cal1(id);
        #pragma omp barrier

        B[id]= big_calc2(id,A);

    }
}