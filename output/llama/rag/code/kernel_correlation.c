#include <math.h>
#include <ap_fixed.h>

void kernel_correlation(int m,int n,float float_n,float data[100][80],float corr[80][80],float mean[80],float stddev[80])
{
  #pragma HLS INTERFACE m_axi port=data offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=corr offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=mean offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=stddev offset=slave bundle=gmem
  #pragma HLS INTERFACE axis port=float_n
  #pragma HLS INTERFACE axis port=m
  #pragma HLS INTERFACE axis port=n

  float eps = 0.1;
  int i;
  int j;
  int k;

  #pragma HLS DATAFLOW
  #pragma HLS PIPELINE II=1

  // Calculate mean
  #pragma HLS LOOP_TRIPCOUNT min=80 max=80
  for (j = 0; j < 80; j++) {
    mean[j] = 0.0;
    #pragma HLS LOOP_TRIPCOUNT min=100 max=100
    for (i = 0; i < 100; i++) {
      mean[j] += data[i][j];
    }
    mean[j] /= float_n;
  }

  // Calculate stddev
  #pragma HLS LOOP_TRIPCOUNT min=80 max=80
  for (j = 0; j < 80; j++) {
    stddev[j] = 0.0;
    #pragma HLS LOOP_TRIPCOUNT min=100 max=100
    for (i = 0; i < 100; i++) {
      stddev[j] += (data[i][j] - mean[j]) * (data[i][j] - mean[j]);
    }
    stddev[j] /= float_n;
    stddev[j] = sqrt(stddev[j]);
    #pragma HLS DEPENDENCE variable=stddev[j] inter false
    #pragma HLS DEPENDENCE variable=stddev[j] intra false
    stddev[j] = (stddev[j] <= eps?1.0 : stddev[j]);
  }

  // Normalize data
  #pragma HLS LOOP_TRIPCOUNT min=100 max=100
  for (i = 0; i < 100; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=80 max=80
    for (j = 0; j < 80; j++) {
      data[i][j] -= mean[j];
      data[i][j] /= sqrt(float_n) * stddev[j];
    }
  }

  // Calculate correlation
  #pragma HLS LOOP_TRIPCOUNT min=80 max=80
  for (i = 0; i < 80 - 1; i++) {
    corr[i][i] = 1.0;
    #pragma HLS LOOP_TRIPCOUNT min=80 max=80
    for (j = i + 1; j < 80; j++) {
      corr[i][j] = 0.0;
      #pragma HLS LOOP_TRIPCOUNT min=100 max=100
      for (k = 0; k < 100; k++) {
        corr[i][j] += data[k][i] * data[k][j];
      }
      corr[j][i] = corr[i][j];
    }
  }
  corr[80 - 1][80 - 1] = 1.0;
}