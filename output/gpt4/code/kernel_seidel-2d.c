```c
void kernel_seidel_2d(int tsteps,int n,float A[120][120])
{
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=A bundle=control
#pragma HLS INTERFACE s_axilite port=tsteps bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control
  int t;
  int i;
  int j;
{
    for (t = 0; t <= 40 - 1; t++) {
#pragma HLS LOOP_TRIPCOUNT min=40 max=40
      for (i = 1; i <= 120 - 2; i++) {
#pragma HLS PIPELINE II=1
        for (j = 1; j <= 120 - 2; j++) {
#pragma HLS UNROLL factor=4
          A[i][j] = (A[i - 1][j - 1] + A[i - 1][j] + A[i - 1][j + 1] + A[i][j - 1] + A[i][j] + A[i][j + 1] + A[i + 1][j - 1] + A[i + 1][j] + A[i + 1][j + 1]) / 9.0;
        }
      }
    }
  }
}
```