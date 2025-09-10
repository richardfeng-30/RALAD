void kernel_gemver(int n, float alpha, float beta, float A[120][120], float u1[120], float v1[120], float u2[120], float v2[120], float w[120], float x[120], float y[120], float z[120]) {
  #pragma HLS INTERFACE m_axi port=A[0] depth=120
  #pragma HLS INTERFACE m_axi port=u1[0] depth=120
  #pragma HLS INTERFACE m_axi port=v1[0] depth=120
  #pragma HLS INTERFACE m_axi port=u2[0] depth=120
  #pragma HLS INTERFACE m_axi port=v2[0] depth=120
  #pragma HLS INTERFACE m_axi port=w[0] depth=120
  #pragma HLS INTERFACE m_axi port=x[0] depth=120
  #pragma HLS INTERFACE m_axi port=y[0] depth=120
  #pragma HLS INTERFACE m_axi port=z[0] depth=120

  #pragma HLS DATAFLOW

  int i;
  int j;

  // Compute A
  #pragma HLS LOOP_FLATTEN
  for (i = 0; i < 120; i++) {
    #pragma HLS LOOP_FLATTEN
    for (j = 0; j < 120; j++) {
      A[i][j] = A[i][j] + u1[i] * v1[j] + u2[i] * v2[j];
    }
  }

  // Compute x
  #pragma HLS LOOP_FLATTEN
  for (i = 0; i < 120; i++) {
    #pragma HLS LOOP_FLATTEN
    for (j = 0; j < 120; j++) {
      x[i] = x[i] + beta * A[j][i] * y[j];
    }
  }

  // Add z to x
  #pragma HLS LOOP_FLATTEN
  for (i = 0; i < 120; i++) {
    x[i] = x[i] + z[i];
  }

  // Compute w
  #pragma HLS LOOP_FLATTEN
  for (i = 0; i < 120; i++) {
    #pragma HLS LOOP_FLATTEN
    for (j = 0; j < 120; j++) {
      w[i] = w[i] + alpha * A[i][j] * x[j];
    }
  }
}