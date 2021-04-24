#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define size 100000000

double serial_sum(double *x)
{
  double sum_val = 0.0;

  for (int i = 0; i < size; i++) {
    sum_val += x[i];
  }

  return sum_val;
}

double omp_sum(double *x)
{
  double sum_val = 0.0;
  
  #pragma omp parallel for
    for (int i = 0; i < size; i++) {
      sum_val += x[i];
    }

  return sum_val;
}

double omp_critical_sum(double *x)
{
  double sum_val = 0.0;
  
  #pragma omp parallel for
    for (int i = 0; i < size; i++) {
      #pragma omp critical
      {
        sum_val += x[i];
      }
    }

  return sum_val;
}

double omp_local_sum(double *x)
{
  double sum_val = 0.0;
  double local_sum[64];
  for(int i=0;i<64;i++){
    local_sum[i] = 0.0;
  }
  #pragma omp parallel shared(local_sum)
  {
    int id = omp_get_thread_num();
    local_sum[id] = 0.0;
    #pragma omp for
        for (int i = 0; i < size; i++) {
            local_sum[id] += x[i];
        }
  }
  for(int i=0;i<64;i++){
    sum_val+=local_sum[i];
  }

  return sum_val;
}

typedef struct{double val; char pad[128];} tsum;

double opt_local_sum(double *x)
{
  double sum_val = 0.0;
  tsum local_sum[64];
  for(int i=0;i<64;i++){
    local_sum[i].val = 0.0;
  }
  #pragma omp parallel shared(local_sum)
  {
    int id = omp_get_thread_num();
    local_sum[id].val = 0.0;
    #pragma omp for
        for (int i = 0; i < size; i++) {
            local_sum[id].val += x[i];
        }
  }
  for(int i=0;i<64;i++){
    sum_val+=local_sum[i].val;
  }

  return sum_val;
}



int main()
{
    double x[size];
    double sum = 0.0;
    double psum = 0.0;
    double csum = 0.0;
    double lsum = 0.0;
    double optsum = 0.0;
    double t1,t2,t3,t4,t5,t6;
    
    for(int i=0;i<size;i++){
        x[i] = rand()/(double)(RAND_MAX);
    }
    t1=omp_get_wtime();
    sum = serial_sum(x);
    t2=omp_get_wtime();
    psum = omp_sum(x);
    t3=omp_get_wtime();
    csum = omp_critical_sum(x);
    t4=omp_get_wtime();
    lsum = omp_local_sum(x);
    t5=omp_get_wtime();
    optsum = opt_local_sum(x);
    t6=omp_get_wtime();
    printf("serial result=%f,parallel result=%f,critical result=%f,local sum result=%f,opt result=%f\n",sum,psum,csum,lsum,optsum);
    printf("serial runtime=%f, parallel runtime=%f, critical runtime=%f, local runtime=%f, opt runtime=%f\n",t2-t1,t3-t2,t4-t3,t5-t4,t6-t5);
    return 0;
}
