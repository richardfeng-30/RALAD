#include <math.h>

void kernel_correlation(int m, int n, float float_n, float data[100][80], float corr[80][80], float mean[80], float stddev[80]) {
  #pragma HLS INTERFACE m_axi port=data offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=corr offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=mean offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=stddev offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=float_n offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axli port=m
  #pragma HLS INTERFACE s_axli port=n
  #pragma HLS INTERFACE s_axli port=float_n
  #pragma HLS INTERFACE s_axli port=corr
  #pragma HLS INTERFACE s_axli port=mean
  #pragma HLS INTERFACE s_axli port=stddev

  float eps = 0.1;
  int i;
  int j;
  int k;

  {
    for (j = 0; j < 80; j++) {
      mean[j] = 0.0;
      #pragma HLS PIPELINE II=100
      for (i = 0; i < 100; i++) {
        mean[j] += data[i][j];
      }
      mean[j] /= float_n;
    }

    for (j = 0; j < 80; j++) {
      stddev[j] = 0.0;
      #pragma HLS PIPELINE II=100
      for (i = 0; i < 100; i++) {
        stddev[j] += (data[i][j] - mean[j]) * (data[i][j] - mean[j]);
      }
      stddev[j] /= float_n;
      stddev[j] = sqrt(stddev[j]);
      if (stddev[j] <= eps) {
        stddev[j] = 1.0;
      }
    }

    for (i = 0; i < 100; i++) {
      for (j = 0; j < 80; j++) {
        data[i][j] -= mean[j];
        data[i][j] /= sqrt(float_n) * stddev[j];
      }
    }

    for (i = 0; i < 80 - 1; i++) {
      corr[i][i] = 1.0;
      #pragma HLS UNROLL
      for (j = i + 1; j < 80; j++) {
        corr[i][j] = 0.0;
        #pragma HLS UNROLL
        for (k = 0; k < 100; k++) {
          corr[i][j] += data[k][i] * data[k][j];
        }
        corr[j][i] = corr[i][j];
      }
    }
    corr[80 - 1][80 - 1] = 1.0;
  }
}