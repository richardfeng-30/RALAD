void kernel_gemver(int n, float alpha, float beta, float A[120][120], float u1[120], float v1[120], float u2[120], float v2[120], float w[120], float x[120], float y[120], float z[120]) {
  #pragma HLS INTERFACE m_axi port=A offset=slave
  #pragma HLS INTERFACE m_axi port=u1 offset=slave
  #pragma HLS INTERFACE m_axi port=v1 offset=slave
  #pragma HLS INTERFACE m_axi port=u2 offset=slave
  #pragma HLS INTERFACE m_axi port=v2 offset=slave
  #pragma HLS INTERFACE m_axi port=w offset=slave
  #pragma HLS INTERFACE m_axi port=x offset=slave
  #pragma HLS INTERFACE m_axi port=y offset=slave
  #pragma HLS INTERFACE m_axi port=z offset=slave
  #pragma HLS INTERFACE s_axli offset=slave bundle=AXILITE

  int i;
  int j;

  // Vector multiplication and matrix addition
  #pragma HLS DATAFLOW
  #pragma HLS LOOP_TRIPCOUNT min = 120 max = 120
  for (i = 0; i < 120; i++) {
    for (j = 0; j < 120; j++) {
      A[i][j] = A[i][j] + u1[i] * v1[j] + u2[i] * v2[j];
    }
  }

  #pragma HLS LOOP_TRIPCOUNT min = 120 max = 120
  for (i = 0; i < 120; i++) {
    for (j = 0; j < 120; j++) {
      x[i] = x[i] + beta * A[j][i] * y[j];
    }
  }

  #pragma HLS LOOP_TRIPCOUNT min = 120 max = 120
  for (i = 0; i < 120; i++) {
    x[i] = x[i] + z[i];
  }

  #pragma HLS LOOP_TRIPCOUNT min = 120 max = 120
  for (i = 0; i < 120; i++) {
    for (j = 0; j < 120; j++) {
      w[i] = w[i] + alpha * A[i][j] * x[j];
    }
  }
}