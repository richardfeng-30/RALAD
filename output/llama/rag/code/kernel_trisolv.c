void kernel_trisolv(int n, float L[120][120], float x[120], float b[120]) {
  #pragma HLS INTERFACE m_axi port=L offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=b offset=slave bundle=gmem

  int i;
  int j;

  #pragma HLS DATAFLOW
  #pragma HLS PIPELINE II=1

  for (i = 0; i < 120; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=1 max=120 avg=60
    #pragma HLS UNROLL factor=1

    x[i] = b[i];
    for (j = 0; j < i; j++) {
      #pragma HLS LOOP_TRIPCOUNT min=1 max=119 avg=59.5
      #pragma HLS UNROLL factor=4

      x[i] -= L[i][j] * x[j];
    }
    x[i] = x[i] / L[i][i];
  }
}