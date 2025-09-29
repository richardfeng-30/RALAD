```c
void kernel_2mm(int ni,int nj,int nk,int nl,float alpha,float beta,float tmp[40][50],float A[40][70],float B[70][50],float C[50][80],float D[40][80])
{
#pragma HLS INLINE off
#pragma HLS ARRAY_PARTITION variable=A complete dim=2
#pragma HLS ARRAY_PARTITION variable=B complete dim=1
#pragma HLS ARRAY_PARTITION variable=tmp complete dim=2
#pragma HLS ARRAY_PARTITION variable=C complete dim=1
#pragma HLS ARRAY_PARTITION variable=D complete dim=2
  int i;
  int j;
  int k;
{
    for (i = 0; i < 40; i++) {
#pragma HLS PIPELINE II=1
      for (j = 0; j < 50; j++) {
#pragma HLS UNROLL factor=2
        tmp[i][j] = 0.0;
        for (k = 0; k < 70; ++k) {
#pragma HLS UNROLL factor=2
          tmp[i][j] += alpha * A[i][k] * B[k][j];
        }
      }
    }
    for (i = 0; i < 40; i++) {
#pragma HLS PIPELINE II=1
      for (j = 0; j < 80; j++) {
#pragma HLS UNROLL factor=2
        D[i][j] *= beta;
        for (k = 0; k < 50; ++k) {
#pragma HLS UNROLL factor=2
          D[i][j] += tmp[i][k] * C[k][j];
        }
      }
    }
  }
}
```