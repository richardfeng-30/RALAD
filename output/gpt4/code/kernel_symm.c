```c
void kernel_symm(int m,int n,float alpha,float beta,float C[60][80],float A[60][60],float B[60][80])
{
#pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem2
#pragma HLS INTERFACE s_axilite port=m bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=alpha bundle=control
#pragma HLS INTERFACE s_axilite port=beta bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS ARRAY_PARTITION variable=A complete dim=2
#pragma HLS ARRAY_PARTITION variable=B complete dim=2
#pragma HLS ARRAY_PARTITION variable=C complete dim=2

  float temp2;
  int i;
  int j;
  int k;

  for (i = 0; i < 60; i++) {
#pragma HLS PIPELINE II=1
    for (j = 0; j < 80; j++) {
#pragma HLS UNROLL factor=4
      temp2 = ((float )0);
      for (k = 0; k < i; k++) {
#pragma HLS UNROLL factor=4
        C[k][j] += alpha * B[i][j] * A[i][k];
        temp2 += B[k][j] * A[i][k];
      }
      C[i][j] = beta * C[i][j] + alpha * B[i][j] * A[i][i] + alpha * temp2;
    }
  }
}
```