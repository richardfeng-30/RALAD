void kernel_lu(int n, float A[120][120]) {
  #pragma HLS interface m_axi port=A[0] bundle=gmem
  int i;
  int j;
  int k;
  float temp;
  
  for (i = 0; i < 120; i++) {
    #pragma HLS loop_tripcount min=1 max=120
    #pragma HLS pipeline II=1
    for (j = 0; j < i; j++) {
      #pragma HLS loop_tripcount min=1 max=120
      #pragma HLS loop_unroll max=8
      for (k = 0; k < j; k++) {
        #pragma HLS unroll
        temp = A[i][k] * A[k][j];
        #pragma HLS dependency variable=temp
        A[i][j] = A[i][j] - temp;
      }
      temp = A[j][j];
      #pragma HLS dependency variable=temp
      A[i][j] = A[i][j] / temp;
    }
    
    for (j = i; j < 120; j++) {
      #pragma HLS loop_tripcount min=1 max=120
      #pragma HLS pipeline II=1
      for (k = 0; k < i; k++) {
        #pragma HLS loop_tripcount min=1 max=120
        #pragma HLS loop_unroll max=8
        temp = A[i][k] * A[k][j];
        #pragma HLS dependency variable=temp
        A[i][j] = A[i][j] - temp;
      }
    }
  }
}