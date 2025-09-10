void kernel_ludcmp(int n,float A[120][120],float b[120],float x[120],float y[120])
{
#pragma HLS array_partition variable=A complete dim=2
#pragma HLS array_partition variable=b complete
#pragma HLS array_partition variable=x complete
#pragma HLS array_partition variable=y complete

  int i;
  int j;
  int k;
  float w;
{
  // Outer loop i can be pipelined with initiation interval 1 to improve throughput
#pragma HLS pipeline II=1
  for (i = 0; i < 120; i++) {
    // Inner loops can be unrolled partially to increase parallelism without excessive resource usage
#pragma HLS unroll factor=4
    for (j = 0; j < i; j++) {
      w = A[i][j];
      for (k = 0; k < j; k++) {
#pragma HLS unroll factor=4
        w -= A[i][k] * A[k][j];
      }
      A[i][j] = w / A[j][j];
    }
#pragma HLS unroll factor=4
    for (j = i; j < 120; j++) {
      w = A[i][j];
      for (k = 0; k < i; k++) {
#pragma HLS unroll factor=4
        w -= A[i][k] * A[k][j];
      }
      A[i][j] = w;
    }
  }

#pragma HLS pipeline II=1
  for (i = 0; i < 120; i++) {
    w = b[i];
#pragma HLS unroll factor=4
    for (j = 0; j < i; j++) {
      w -= A[i][j] * y[j];
    }
    y[i] = w;
  }

#pragma HLS pipeline II=1
  for (i = 0; i <= 119; i++) {
    int _in_i = 119 + -1 * i;
    w = y[_in_i];
#pragma HLS unroll factor=4
    for (j = _in_i + 1; j < 120; j++) {
      w -= A[_in_i][j] * x[j];
    }
    x[_in_i] = w / A[_in_i][_in_i];
  }
  i = 0 + -1;
  }
}