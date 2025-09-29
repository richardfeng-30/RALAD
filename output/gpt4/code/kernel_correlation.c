```c
#include <math.h>

void kernel_correlation(int m,int n,float float_n,float data[100][80],float corr[80][80],float mean[80],float stddev[80])
{
#pragma HLS INTERFACE m_axi port=data offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=corr offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=mean offset=slave bundle=gmem2
#pragma HLS INTERFACE m_axi port=stddev offset=slave bundle=gmem3
#pragma HLS INTERFACE s_axilite port=m bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=float_n bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS ARRAY_PARTITION variable=data complete dim=2
#pragma HLS ARRAY_PARTITION variable=mean complete dim=1
#pragma HLS ARRAY_PARTITION variable=stddev complete dim=1
#pragma HLS ARRAY_PARTITION variable=corr complete dim=2

  float eps = 0.1;
  int i;
  int j;
  int k;

  // Mean calculation
  mean_loop_j: for (j = 0; j < 80; j++) {
#pragma HLS PIPELINE II=1
    mean[j] = 0.0;
    mean_loop_i: for (i = 0; i < 100; i++) {
#pragma HLS UNROLL factor=4
      mean[j] += data[i][j];
    }
    mean[j] /= float_n;
  }

  // Stddev calculation
  stddev_loop_j: for (j = 0; j < 80; j++) {
#pragma HLS PIPELINE II=1
    stddev[j] = 0.0;
    stddev_loop_i: for (i = 0; i < 100; i++) {
#pragma HLS UNROLL factor=4
      stddev[j] += (data[i][j] - mean[j]) * (data[i][j] - mean[j]);
    }
    stddev[j] /= float_n;
    stddev[j] = sqrt(stddev[j]);
    stddev[j] = (stddev[j] <= eps?1.0 : stddev[j]);
  }

  // Data normalization
  norm_loop_i: for (i = 0; i < 100; i++) {
#pragma HLS PIPELINE II=1
    norm_loop_j: for (j = 0; j < 80; j++) {
#pragma HLS UNROLL factor=4
      data[i][j] -= mean[j];
      data[i][j] /= sqrt(float_n) * stddev[j];
    }
  }

  // Correlation calculation
  corr_loop_i: for (i = 0; i < 80 - 1; i++) {
#pragma HLS PIPELINE II=1
    corr[i][i] = 1.0;
    corr_loop_j: for (j = i + 1; j < 80; j++) {
      corr[i][j] = 0.0;
      corr_loop_k: for (k = 0; k < 100; k++) {
#pragma HLS UNROLL factor=4
        corr[i][j] += data[k][i] * data[k][j];
      }
      corr[j][i] = corr[i][j];
    }
  }
  corr[80 - 1][80 - 1] = 1.0;
}
```