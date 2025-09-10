void kernel_lu(int n, float A[120][120]) {
  #pragma HLS interface m_axi port=A[0] depth=120 dim1=120 dim2=120
  #pragma HLS interface m_axi port=A[1] depth=120 dim1=120 dim2=120
  
  int i;
  int j;
  int k;
  
  for (i = 0; i < 120; i++) {
    #pragma HLS loop_tripcount min=120 max=120 avg=120
    for (j = 0; j < i; j++) {
      #pragma HLS loop_tripcount min=120 max=120 avg=120
      for (k = 0; k < j; k++) {
        #pragma HLS pipeline II=1
        A[i][j] -= A[i][k] * A[k][j];
      }
      #pragma HLS pipeline II=1
      A[i][j] /= A[j][j];
    }
    
    for (j = i; j < 120; j++) {
      #pragma HLS loop_tripcount min=120 max=120 avg=120
      for (k = 0; k < i; k++) {
        #pragma HLS pipeline II=1
        A[i][j] -= A[i][k] * A[k][j];
      }
    }
  }
}