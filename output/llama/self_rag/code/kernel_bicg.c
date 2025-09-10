void kernel_bicg(int m, int n, float A[124][116], float s[116], float q[124], float p[116], float r[124]) {
  #pragma HLS INTERFACE m_axi port=A
  #pragma HLS INTERFACE m_axi port=s
  #pragma HLS INTERFACE m_axi port=q
  #pragma HLS INTERFACE m_axi port=p
  #pragma HLS INTERFACE m_axi port=r

  int i;
  int j;

  // Initialize s
  #pragma HLS PIPELINE II=1
  for (i = 0; i < 116; i++) {
    s[i] = 0.0;
  }

  // Update s and q
  #pragma HLS PIPELINE II=1
  for (i = 0; i < 124; i++) {
    q[i] = 0.0;
    #pragma HLS LOOP_FLATTEN
    for (j = 0; j < 116; j++) {
      #pragma HLS DEPENDENCE variable=s false
      s[j] = s[j] + r[i] * A[i][j];
      q[i] = q[i] + A[i][j] * p[j];
    }
  }
}