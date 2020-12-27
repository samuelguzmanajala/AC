#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

static long num_steps = 100000;
double step; 
int main()
{
    int nth = omp_get_num_threads();

    int i; double x, pi, sum = 0.0;

#pragma omp parallel private(i,x,pi,sum,step)

    step = 1.0/(double)(num_steps/nth);

#pragma omp for

    for(i = 0; i<num_steps/nth; i++)
    {
        x=(i+0.5)*step;
        sum += 4.0/(1.0+x*x);
    }

    pi = step*sum;

}
