void kernel_mvt(int n, float x1[120], float x2[120], float y_1[120], float y_2[120], float A[120][120]) {
  int i;
  int j;
  
  #pragma HLS INTERFACE m_axi port=x1 offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=x2 offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=y_1 offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=y_2 offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  
  {
    #pragma HLS DATAFLOW
    //Matrix-Vector Product
    for (i = 0; i < 120; i++) {
      #pragma HLS LOOP_TRIPCOUNT min = 120 max = 120
      for (j = 0; j < 120; j++) {
        #pragma HLS LOOP_TRIPCOUNT min = 120 max = 120
        x1[i] = x1[i] + A[i][j] * y_1[j];
      }
    }
    
    //Transpose
    for (i = 0; i < 120; i++) {
      #pragma HLS LOOP_TRIPCOUNT min = 120 max = 120
      for (j = 0; j < 120; j++) {
        #pragma HLS LOOP_TRIPCOUNT min = 120 max = 120
        x2[i] = x2[i] + A[j][i] * y_2[j];
      }
    }
  }
}