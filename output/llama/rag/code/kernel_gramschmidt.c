#include <math.h>

void kernel_gramschmidt(int m, int n, float A[60][80], float R[80][80], float Q[60][80]) {
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=R offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=Q offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axli port=m
  #pragma HLS INTERFACE s_axli port=n
  #pragma HLS INTERFACE s_axli port=A
  #pragma HLS INTERFACE s_axli port=R
  #pragma HLS INTERFACE s_axli port=Q
  #pragma HLS INTERFACE s_axli port=return

  int i;
  int j;
  int k;
  float nrm;

  #pragma HLS DATAFLOW

  // First loop
  #pragma HLS LOOP_TRIPCOUNT min=80 max=80
  #pragma HLS PIPELINE II=1
  for (k = 0; k < 80; k++) {
    nrm = 0.0;

    #pragma HLS LOOP_TRIPCOUNT min=60 max=60
    #pragma HLS PIPELINE II=1
    for (i = 0; i < 60; i++) {
      nrm += A[i][k] * A[i][k];
    }
    R[k][k] = sqrt(nrm);

    #pragma HLS LOOP_TRIPCOUNT min=60 max=60
    #pragma HLS PIPELINE II=1
    for (i = 0; i < 60; i++) {
      Q[i][k] = A[i][k] / R[k][k];
    }
  }

  // Second loop
  #pragma HLS LOOP_TRIPCOUNT min=79 max=79
  #pragma HLS PIPELINE II=1
  for (k = 0; k < 80; k++) {
    #pragma HLS LOOP_TRIPCOUNT min=60 max=60
    #pragma HLS PIPELINE II=1
    for (j = k + 1; j < 80; j++) {
      R[k][j] = 0.0;

      #pragma HLS LOOP_TRIPCOUNT min=60 max=60
      #pragma HLS PIPELINE II=1
      for (i = 0; i < 60; i++) {
        R[k][j] += Q[i][k] * A[i][j];
      }

      #pragma HLS LOOP_TRIPCOUNT min=60 max=60
      #pragma HLS PIPELINE II=1
      for (i = 0; i < 60; i++) {
        A[i][j] = A[i][j] - Q[i][k] * R[k][j];
      }
    }
  }
}