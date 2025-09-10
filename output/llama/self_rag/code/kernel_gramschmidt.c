#include <math.h>

void kernel_gramschmidt(int m, int n, float A[60][80], float R[80][80], float Q[60][80]) {
  #pragma HLS INTERFACE m_axi port=A offset=slave
  #pragma HLS INTERFACE m_axi port=R offset=slave
  #pragma HLS INTERFACE m_axi port=Q offset=slave
  #pragma HLS INTERFACE s_axilite port=m
  #pragma HLS INTERFACE s_axilite port=n
  #pragma HLS INTERFACE s_axilite port=return

  int i;
  int j;
  int k;
  float nrm;
  #pragma HLS DATAFLOW

  // Compute R and Q
  #pragma HLS LOOP_TRIPCOUNT min=80 max=80
  for (k = 0; k < 80; k++) {
    #pragma HLS LOOP_TRIPCOUNT min=60 max=60
    nrm = 0.0;
    #pragma HLS UNROLL
    for (i = 0; i < 60; i++) {
      nrm += A[i][k] * A[i][k];
    }
    R[k][k] = sqrt(nrm);
    
    #pragma HLS LOOP_TRIPCOUNT min=60 max=60
    for (i = 0; i < 60; i++) {
      Q[i][k] = A[i][k] / R[k][k];
    }
  }

  // Compute R and update A and Q
  #pragma HLS LOOP_TRIPCOUNT min=79 max=79
  for (k = 0; k < 80; k++) {
    #pragma HLS LOOP_TRIPCOUNT min=60 max=60
    R[k][k] = 1.0;
    #pragma HLS UNROLL
    for (j = k + 1; j < 80; j++) {
      R[k][j] = 0.0;
      #pragma HLS UNROLL
      for (i = 0; i < 60; i++) {
        R[k][j] += Q[i][k] * A[i][j];
      }
      
      #pragma HLS UNROLL
      for (i = 0; i < 60; i++) {
        A[i][j] = A[i][j] - Q[i][k] * R[k][j];
      }
    }
  }
}