#include <math.h>

void kernel_gramschmidt(int m,int n,float A[60][80],float R[80][80],float Q[60][80])
{
  #pragma HLS INTERFACE m_axi port=A[0] offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=R[0] offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=Q[0] offset=slave bundle=gmem
  
  int i;
  int j;
  int k;
  float nrm;
  
  #pragma HLS DATAFLOW
  
  #pragma HLS LOOP_TRIPCOUNT min=1 max=80 avg=40
  
  for (k = 0; k < 80; k++) {
    #pragma HLS LOOP_TRIPCOUNT min=1 max=1 avg=1
    nrm = 0.0;
    
    #pragma HLS PIPELINE II=1
    for (i = 0; i < 60; i++) {
      nrm += A[i][k] * A[i][k];
    }
    R[k][k] = sqrt(nrm);
    
    #pragma HLS PIPELINE II=1
    for (i = 0; i < 60; i++) {
      Q[i][k] = A[i][k] / R[k][k];
    }
    
    #pragma HLS LOOP_TRIPCOUNT min=1 max=79 avg=40
    for (j = k + 1; j < 80; j++) {
      R[k][j] = 0.0;
      
      #pragma HLS LOOP_TRIPCOUNT min=1 max=60 avg=30
      for (i = 0; i < 60; i++) {
        R[k][j] += Q[i][k] * A[i][j];
      }
      
      #pragma HLS LOOP_TRIPCOUNT min=1 max=60 avg=30
      for (i = 0; i < 60; i++) {
        A[i][j] = A[i][j] - Q[i][k] * R[k][j];
      }
    }
  }
}