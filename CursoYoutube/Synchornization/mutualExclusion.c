#include <omp.h>

void main()
{
    float res; 
    #pragma omp parallel 
    {
        float B; int i, id, nthrds; 
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        for(i = id; i < nieters; i+= nthrds){
            B = big_job(i);
            #pragma omp critical
                res+= consume(B);
        }

    }

}
