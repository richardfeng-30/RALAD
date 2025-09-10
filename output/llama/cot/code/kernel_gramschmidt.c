#include <math.h>

void kernel_gramschmidt(int m,int n,float A[60][80],float R[80][80],float Q[60][80])
{
  #pragma HLS dataflow
  #pragma HLS pipeline II=1

  int i;
  int j;
  int k;
  float nrm;
  
  for (k = 0; k < 80; k++) {
    #pragma HLS loop_tripcount min=1 max=80
    #pragma HLS unroll
    nrm = 0.0;
    
    for (i = 0; i < 60; i++) {
      #pragma HLS loop_tripcount min=1 max=60
      #pragma HLS unroll
      nrm += A[i][k] * A[i][k];
    }
    R[k][k] = sqrt(nrm);
    
    for (i = 0; i < 60; i++) {
      #pragma HLS loop_tripcount min=1 max=60
      #pragma HLS unroll
      Q[i][k] = A[i][k] / R[k][k];
    }
    
    for (j = k + 1; j < 80; j++) {
      #pragma HLS loop_tripcount min=1 max=79
      #pragma HLS unroll
      R[k][j] = 0.0;
      
      for (i = 0; i < 60; i++) {
        #pragma HLS loop_tripcount min=1 max=60
        #pragma HLS unroll
        R[k][j] += Q[i][k] * A[i][j];
      }
      
      for (i = 0; i < 60; i++) {
        #pragma HLS loop_tripcount min=1 max=60
        #pragma HLS unroll
        A[i][j] = A[i][j] - Q[i][k] * R[k][j];
      }
    }
  }
}

#pragma HLS resource variable A T:8 storage:RAM
#pragma HLS resource variable R T:8 storage:RAM
#pragma HLS resource variable Q T:8 storage:RAM