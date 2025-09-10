void kernel_lu(int n, float A[120][120]) {
  #pragma HLS INTERFACE m_axi port=A[0] depth=120 dim=2
  #pragma HLS INTERFACE s_axilite port=n
  #pragma HLS INTERFACE s_axilite port=return

  int i;
  int j;
  int k;
  
  for (i = 0; i < 120; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=1 max=120 avg=60
    #pragma HLS PIPELINE II=1
    for (j = 0; j < i; j++) {
      #pragma HLS LOOP_TRIPCOUNT min=1 max=119 avg=59
      for (k = 0; k < j; k++) {
        #pragma HLS UNROLL
        A[i][j] -= A[i][k] * A[k][j];
      }
      #pragma HLS PIPELINE II=1
      A[i][j] /= A[j][j];
    }
    
    for (j = i; j < 120; j++) {
      #pragma HLS LOOP_TRIPCOUNT min=1 max=81 avg=40
      #pragma HLS PIPELINE II=1
      for (k = 0; k < i; k++) {
        #pragma HLS UNROLL
        A[i][j] -= A[i][k] * A[k][j];
      }
    }
  }
}