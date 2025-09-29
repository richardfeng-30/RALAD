```c
void kernel_covariance(int m,int n,float float_n,float data[100][80],float cov[80][80],float mean[80])
{
#pragma HLS INTERFACE m_axi port=data offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=cov offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=mean offset=slave bundle=gmem2
#pragma HLS INTERFACE s_axilite port=m bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=float_n bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS ARRAY_PARTITION variable=data cyclic factor=8 dim=2
#pragma HLS ARRAY_PARTITION variable=cov cyclic factor=8 dim=2
#pragma HLS ARRAY_PARTITION variable=mean complete dim=1

  int i;
  int j;
  int k;
  {

    // Mean calculation
    for (j = 0; j < 80; j++) {
#pragma HLS PIPELINE II=1
      mean[j] = 0.0;
      for (i = 0; i < 100; i++) {
#pragma HLS UNROLL factor=4
        mean[j] += data[i][j];
      }
      mean[j] /= float_n;
    }

    // Data centering
    for (i = 0; i < 100; i++) {
#pragma HLS PIPELINE II=1
      for (j = 0; j < 80; j++) {
#pragma HLS UNROLL factor=8
        data[i][j] -= mean[j];
      }
    }

    // Covariance calculation
    for (i = 0; i < 80; i++) {
#pragma HLS PIPELINE II=1
      for (j = i; j < 80; j++) {
        cov[i][j] = 0.0;
        for (k = 0; k < 100; k++) {
#pragma HLS UNROLL factor=4
          cov[i][j] += data[k][i] * data[k][j];
        }
        cov[i][j] /= float_n - 1.0;
        cov[j][i] = cov[i][j];
      }
    }
  }
}
```