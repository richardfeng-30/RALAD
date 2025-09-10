void kernel_ludcmp(int n,float A[120][120],float b[120],float x[120],float y[120])
{
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=b offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axli port=n
  #pragma HLS INTERFACE s_axli port=return

  int i;
  int j;
  int k;
  float w;
  #pragma HLS DATAFLOW

  // Forward Elimination
  #pragma HLS LOOP_TRIPCOUNT min=120 max=120
  for (i = 0; i < 120; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=i max=i
    for (j = 0; j < i; j++) {
      w = A[i][j];
      #pragma HLS LOOP_TRIPCOUNT min=j max=j
      for (k = 0; k < j; k++) {
        w -= A[i][k] * A[k][j];
      }
      A[i][j] = w / A[j][j];
    }
    #pragma HLS LOOP_TRIPCOUNT min=i max=i
    for (j = i; j < 120; j++) {
      w = A[i][j];
      #pragma HLS LOOP_TRIPCOUNT min=i max=i
      for (k = 0; k < i; k++) {
        w -= A[i][k] * A[k][j];
      }
      A[i][j] = w;
    }
  }

  // Solve for y
  #pragma HLS LOOP_TRIPCOUNT min=120 max=120
  for (i = 0; i < 120; i++) {
    w = b[i];
    #pragma HLS LOOP_TRIPCOUNT min=i max=i
    for (j = 0; j < i; j++) {
      w -= A[i][j] * y[j];
    }
    y[i] = w;
  }

  // Back Substitution
  #pragma HLS LOOP_TRIPCOUNT min=120 max=120
  for (i = 0; i <= 119; i++) {
    int _in_i = 119 + -1 * i;
    w = y[_in_i];
    #pragma HLS LOOP_TRIPCOUNT min=_in_i max=_in_i
    for (j = _in_i + 1; j < 120; j++) {
      w -= A[_in_i][j] * x[j];
    }
    x[_in_i] = w / A[_in_i][_in_i];
  }
}