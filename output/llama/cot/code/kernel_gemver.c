void kernel_gemver(int n,float alpha,float beta,float A[120][120],float u1[120],float v1[120],float u2[120],float v2[120],float w[120],float x[120],float y[120],float z[120])
{
  #pragma HLS interface m_axi port=A[0] bundle=gmem
  #pragma HLS interface m_axi port=u1[0] bundle=gmem
  #pragma HLS interface m_axi port=v1[0] bundle=gmem
  #pragma HLS interface m_axi port=u2[0] bundle=gmem
  #pragma HLS interface m_axi port=v2[0] bundle=gmem
  #pragma HLS interface m_axi port=w[0] bundle=gmem
  #pragma HLS interface m_axi port=x[0] bundle=gmem
  #pragma HLS interface m_axi port=y[0] bundle=gmem
  #pragma HLS interface m_axi port=z[0] bundle=gmem

  int i;
  int j;
  {
    #pragma HLS loop_unroll factor=4
    for (i = 0; i < 120; i++) {
      for (j = 0; j < 120; j++) {
        A[i][j] = A[i][j] + u1[i] * v1[j] + u2[i] * v2[j];
      }
    }
  }
  {
    #pragma HLS loop_unroll factor=4
    for (i = 0; i < 120; i++) {
      for (j = 0; j < 120; j++) {
        x[i] = x[i] + beta * A[j][i] * y[j];
      }
    }
  }
  {
    #pragma HLS loop_unroll factor=4
    for (i = 0; i < 120; i++) {
      x[i] = x[i] + z[i];
    }
  }
  {
    #pragma HLS loop_unroll factor=4
    for (i = 0; i < 120; i++) {
      for (j = 0; j < 120; j++) {
        w[i] = w[i] + alpha * A[i][j] * x[j];
      }
    }
  }
}