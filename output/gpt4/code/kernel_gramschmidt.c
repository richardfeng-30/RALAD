```c
#include <math.h>

void kernel_gramschmidt(int m,int n,float A[60][80],float R[80][80],float Q[60][80])
{
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=R offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=Q offset=slave bundle=gmem2
#pragma HLS INTERFACE s_axilite port=A bundle=control
#pragma HLS INTERFACE s_axilite port=R bundle=control
#pragma HLS INTERFACE s_axilite port=Q bundle=control
#pragma HLS INTERFACE s_axilite port=m bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS ARRAY_PARTITION variable=A cyclic factor=8 dim=2
#pragma HLS ARRAY_PARTITION variable=R cyclic factor=8 dim=2
#pragma HLS ARRAY_PARTITION variable=Q cyclic factor=8 dim=2

  int i;
  int j;
  int k;
  float nrm;
{
    for (k = 0; k < 80; k++) {
      nrm = 0.0;
#pragma HLS PIPELINE II=1
      for (i = 0; i < 60; i++) {
#pragma HLS UNROLL factor=4
        nrm += A[i][k] * A[i][k];
      }
      R[k][k] = sqrt(nrm);

      for (i = 0; i < 60; i++) {
#pragma HLS UNROLL factor=4
        Q[i][k] = A[i][k] / R[k][k];
      }

      for (j = k + 1; j < 80; j++) {
        R[k][j] = 0.0;

        for (i = 0; i < 60; i++) {
#pragma HLS UNROLL factor=4
          R[k][j] += Q[i][k] * A[i][j];
        }

        for (i = 0; i < 60; i++) {
#pragma HLS UNROLL factor=4
          A[i][j] = A[i][j] - Q[i][k] * R[k][j];
        }
      }
    }
  }
}
```