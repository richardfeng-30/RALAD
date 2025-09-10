void kernel_syr2k(int n, int m, float alpha, float beta, float C[80][80], float A[80][60], float B[80][60])
{
  #pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
  
  int i;
  int j;
  int k;
  
  #pragma HLS DATAFLOW
  
  for (i = 0; i < 80; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=1 max=80 avg=40
    #pragma HLS UNROLL factor=4
    for (j = 0; j <= i; j++) {
      #pragma HLS LOOP_TRIPCOUNT min=1 max=40 avg=20
      C[i][j] *= beta;
    }
    
    for (k = 0; k < 60; k++) {
      #pragma HLS LOOP_TRIPCOUNT min=1 max=60 avg=30
      for (j = 0; j <= i; j++) {
        #pragma HLS LOOP_TRIPCOUNT min=1 max=40 avg=20
        C[i][j] += A[j][k] * alpha * B[i][k] + B[j][k] * alpha * A[i][k];
      }
    }
  }
}