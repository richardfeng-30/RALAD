void kernel_trisolv(int n, float L[120][120], float x[120], float b[120]) {
  #pragma HLS interface m_axi port = L[0] bundle = s_axi
  #pragma HLS interface m_axi port = x[0] bundle = s_axi
  #pragma HLS interface m_axi port = b[0] bundle = s_axi

  int i;
  int j;
  float acc = 0;

  #pragma HLS dataflow

  #pragma HLS loop_tripcount min = 120 max = 120
  #pragma HLS pipeline II = 1

  for (i = 0; i < 120; i++) {
    x[i] = b[i];
    #pragma HLS loop_tripcount min = i max = i
    #pragma HLS pipeline II = 1
    for (j = 0; j < i; j++) {
      x[i] -= L[i][j] * x[j];
    }
    x[i] = x[i] / L[i][i];
  }
}