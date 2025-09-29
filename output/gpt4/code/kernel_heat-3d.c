```c
void kernel_heat_3d(int tsteps,int n,float A[20][20][20],float B[20][20][20])
{
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem1
#pragma HLS INTERFACE s_axilite port=A bundle=control
#pragma HLS INTERFACE s_axilite port=B bundle=control
#pragma HLS INTERFACE s_axilite port=tsteps bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  int t;
  int i;
  int j;
  int k;
{
    for (t = 1; t <= 40; t++) {
#pragma HLS LOOP_TRIPCOUNT min=40 max=40
      for (i = 1; i < 20 - 1; i++) {
#pragma HLS PIPELINE II=1
        for (j = 1; j < 20 - 1; j++) {
#pragma HLS UNROLL factor=2
          for (k = 1; k < 20 - 1; k++) {
            B[i][j][k] = 0.125 * (A[i + 1][j][k] - 2.0 * A[i][j][k] + A[i - 1][j][k]) + 0.125 * (A[i][j + 1][k] - 2.0 * A[i][j][k] + A[i][j - 1][k]) + 0.125 * (A[i][j][k + 1] - 2.0 * A[i][j][k] + A[i][j][k - 1]) + A[i][j][k];
          }
        }
      }
      for (i = 1; i < 20 - 1; i++) {
#pragma HLS PIPELINE II=1
        for (j = 1; j < 20 - 1; j++) {
#pragma HLS UNROLL factor=2
          for (k = 1; k < 20 - 1; k++) {
            A[i][j][k] = 0.125 * (B[i + 1][j][k] - 2.0 * B[i][j][k] + B[i - 1][j][k]) + 0.125 * (B[i][j + 1][k] - 2.0 * B[i][j][k] + B[i][j - 1][k]) + 0.125 * (B[i][j][k + 1] - 2.0 * B[i][j][k] + B[i][j][k - 1]) + B[i][j][k];
          }
        }
      }
    }
  }
}
```