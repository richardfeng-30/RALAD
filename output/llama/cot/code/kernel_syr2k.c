void kernel_syr2k(int n, int m, float alpha, float beta, float C[80][80], float A[80][60], float B[80][60]) {
  #pragma HLS interface m_axi port=C[0] bundle=gmem latency=0
  #pragma HLS interface m_axi port=A[0] bundle=gmem latency=0
  #pragma HLS interface m_axi port=B[0] bundle=gmem latency=0

  int i;
  int j;
  int k;

  #pragma HLS loop_tripcount max=80
  for (i = 0; i < 80; i++) {
    #pragma HLS loop_tripcount max=i+1
    for (j = 0; j <= i; j++) {
      C[i][j] *= beta;
    }

    #pragma HLS loop_tripcount max=60
    for (k = 0; k < 60; k++) {
      #pragma HLS loop_unroll max=8
      for (j = 0; j <= i; j++) {
        C[i][j] += A[j][k] * alpha * B[i][k] + B[j][k] * alpha * A[i][k];
      }
    }
  }
}