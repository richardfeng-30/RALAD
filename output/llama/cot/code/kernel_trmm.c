void kernel_trmm(int m,int n,float alpha,float A[60][60],float B[60][80])
{
  #pragma HLS interface m_axi port=A[0] bundle=gmem
  #pragma HLS interface m_axi port=B[0] bundle=gmem
  #pragma HLS dataflow
  
  int i;
  int j;
  int k;
  
  #pragma HLS loop_flatten
  for (i = 0; i < 60; i++) {
    #pragma HLS pipeline II=1
    for (j = 0; j < 80; j++) {
      float temp = 0.0f;
      for (k = i + 1; k < 60; k++) {
        #pragma HLS pipeline II=1
        temp += A[k][i] * B[k][j];
      }
      B[i][j] = alpha * temp;
    }
  }
}