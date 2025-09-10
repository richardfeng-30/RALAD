void kernel_ludcmp(int n, float A[120][120], float b[120], float x[120], float y[120]) {
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=b offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem

  int i;
  int j;
  int k;
  float w;
  #pragma HLS DATAFLOW

  // Gaussian Elimination
  for (i = 0; i < 120; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=120 max=120
    #pragma HLS PIPELINE II=1
    for (j = 0; j < i; j++) {
      w = A[i][j];
      for (k = 0; k < j; k++) {
        #pragma HLS UNROLL
        w -= A[i][k] * A[k][j];
      }
      A[i][j] = w / A[j][j];
    }
    for (j = i; j < 120; j++) {
      w = A[i][j];
      for (k = 0; k < i; k++) {
        #pragma HLS UNROLL
        w -= A[i][k] * A[k][j];
      }
      A[i][j] = w;
    }
  }

  // Forward Elimination
  for (i = 0; i < 120; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=120 max=120
    #pragma HLS PIPELINE II=1
    w = b[i];
    for (j = 0; j < i; j++) {
      #pragma HLS UNROLL
      w -= A[i][j] * y[j];
    }
    y[i] = w;
  }

  // Backward Substitution
  for (i = 119; i >= 0; i--) {
    #pragma HLS LOOP_TRIPCOUNT min=120 max=120
    #pragma HLS PIPELINE II=1
    w = y[i];
    for (j = i + 1; j < 120; j++) {
      #pragma HLS UNROLL
      w -= A[i][j] * x[j];
    }
    x[i] = w / A[i][i];
  }
}