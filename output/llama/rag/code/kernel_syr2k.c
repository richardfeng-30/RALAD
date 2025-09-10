void kernel_syr2k(int n, int m, float alpha, float beta, float C[80][80], float A[80][60], float B[80][60])
{
  #pragma HLS interface m_axi port=C[0] bundle=gmem
  #pragma HLS interface m_axi port=A[0] bundle=gmem
  #pragma HLS interface m_axi port=B[0] bundle=gmem

  int i;
  int j;
  int k;

  //BLAS PARAMS
  //UPLO  = 'L'
  //TRANS = 'N'
  //A is NxM
  //B is NxM
  //C is NxN

  {
    for (i = 0; i < 80; i++) {
      #pragma HLS loop_tripcount min=1 max=80
      #pragma HLS pipeline II=1

      for (j = 0; j <= i; j++) {
        #pragma HLS unroll
        C[i][j] *= beta;
      }

      for (k = 0; k < 60; k++) {
        #pragma HLS loop_tripcount min=1 max=60
        #pragma HLS pipeline II=1

        for (j = 0; j <= i; j++) {
          #pragma HLS unroll
          C[i][j] += A[j][k] * alpha * B[i][k] + B[j][k] * alpha * A[i][k];
        }
      }
    }
  }
}