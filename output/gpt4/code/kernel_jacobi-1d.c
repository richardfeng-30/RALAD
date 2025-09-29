```c
void kernel_jacobi_1d(int tsteps,int n,float A[120],float B[120])
{
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=A bundle=control
#pragma HLS INTERFACE s_axilite port=B bundle=control
#pragma HLS INTERFACE s_axilite port=tsteps bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control
#pragma HLS ARRAY_PARTITION variable=A complete dim=1
#pragma HLS ARRAY_PARTITION variable=B complete dim=1

  int t;
  int i;
  {
    for (t = 0; t < 40; t++) {
#pragma HLS PIPELINE II=1
      for (i = 1; i < 120 - 1; i++) {
#pragma HLS UNROLL factor=8
        B[i] = 0.33333 * (A[i - 1] + A[i] + A[i + 1]);
      }
      for (i = 1; i < 120 - 1; i++) {
#pragma HLS UNROLL factor=8
        A[i] = 0.33333 * (B[i - 1] + B[i] + B[i + 1]);
      }
    }
  }
}
```