```c
void kernel_syrk(int n,int m,float alpha,float beta,float C[80][80],float A[80][60])
{
#pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem0 depth=6400
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem1 depth=4800
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=m bundle=control
#pragma HLS INTERFACE s_axilite port=alpha bundle=control
#pragma HLS INTERFACE s_axilite port=beta bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS ARRAY_PARTITION variable=A complete dim=2
#pragma HLS ARRAY_PARTITION variable=C cyclic factor=8 dim=2

  int i;
  int j;
  int k;

  for (i = 0; i < 80; i++) {
#pragma HLS LOOP_TRIPCOUNT min=80 max=80
    for (j = 0; j <= i; j++) {
#pragma HLS PIPELINE II=1
      C[i][j] *= beta;
    }

    for (k = 0; k < 60; k++) {
#pragma HLS LOOP_TRIPCOUNT min=60 max=60
      for (j = 0; j <= i; j++) {
#pragma HLS UNROLL factor=8
        C[i][j] += alpha * A[i][k] * A[j][k];
      }
    }
  }
}
```