```c
void kernel_ludcmp(int n,float A[120][120],float b[120],float x[120],float y[120])
{
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=b offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem2
#pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem3
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control
#pragma HLS ARRAY_PARTITION variable=A cyclic factor=8 dim=2
#pragma HLS ARRAY_PARTITION variable=A cyclic factor=8 dim=1
#pragma HLS ARRAY_PARTITION variable=b cyclic factor=8 dim=1
#pragma HLS ARRAY_PARTITION variable=x cyclic factor=8 dim=1
#pragma HLS ARRAY_PARTITION variable=y cyclic factor=8 dim=1

  int i;
  int j;
  int k;
  float w;
{
    for (i = 0; i < 120; i++) {
#pragma HLS PIPELINE II=1
      for (j = 0; j < i; j++) {
#pragma HLS UNROLL factor=4
        w = A[i][j];
        for (k = 0; k < j; k++) {
#pragma HLS UNROLL factor=4
          w -= A[i][k] * A[k][j];
        }
        A[i][j] = w / A[j][j];
      }
      for (j = i; j < 120; j++) {
#pragma HLS UNROLL factor=4
        w = A[i][j];
        for (k = 0; k < i; k++) {
#pragma HLS UNROLL factor=4
          w -= A[i][k] * A[k][j];
        }
        A[i][j] = w;
      }
    }

    for (i = 0; i < 120; i++) {
#pragma HLS PIPELINE II=1
      w = b[i];
      for (j = 0; j < i; j++) {
#pragma HLS UNROLL factor=4
        w -= A[i][j] * y[j];
      }
      y[i] = w;
    }

    for (i = 0; i <= 119; i++) {
#pragma HLS PIPELINE II=1
      int _in_i = 119 + -1 * i;
      w = y[_in_i];
      for (j = _in_i + 1; j < 120; j++) {
#pragma HLS UNROLL factor=4
        w -= A[_in_i][j] * x[j];
      }
      x[_in_i] = w / A[_in_i][_in_i];
    }
    i = 0 + -1;
  }
}
```