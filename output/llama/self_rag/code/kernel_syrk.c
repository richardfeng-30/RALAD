void kernel_syrk(int n, int m, float alpha, float beta, float C[80][80], float A[80][60])
{
  #pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  
  int i;
  int j;
  int k;
  
  for (i = 0; i < 80; i++) {
    #pragma HLS LOOP_FLATTEN
    for (j = 0; j <= i; j++) {
      #pragma HLS PIPELINE II=1
      C[i][j] *= beta;
    }
    
    for (k = 0; k < 60; k++) {
      #pragma HLS LOOP_FLATTEN
      for (j = 0; j <= i; j++) {
        #pragma HLS PIPELINE II=1
        C[i][j] += alpha * A[i][k] * A[j][k];
      }
    }
  }
}