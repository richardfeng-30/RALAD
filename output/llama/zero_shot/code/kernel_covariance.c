void kernel_covariance(int m,int n,float float_n,float data[100][80],float cov[80][80],float mean[80])
{
  #pragma HLS INTERFACE m_axi port=data offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=cov offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=mean offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=float_n offset=slave bundle=gmem
  
  int i;
  int j;
  int k;
  
  #pragma HLS DATAFLOW
  
  #pragma HLS LOOP_TRIPCOUNT min=100 max=100
  #pragma HLS UNROLL factor=1
  for (j = 0; j < 80; j++) {
    mean[j] = 0.0;
    #pragma HLS LOOP_TRIPCOUNT min=100 max=100
    #pragma HLS UNROLL factor=1
    for (i = 0; i < 100; i++) {
      mean[j] += data[i][j];
    }
    mean[j] /= float_n;
  }
  
  #pragma HLS LOOP_TRIPCOUNT min=100 max=100
  #pragma HLS UNROLL factor=1
  for (i = 0; i < 100; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=80 max=80
    #pragma HLS UNROLL factor=1
    for (j = 0; j < 80; j++) {
      data[i][j] -= mean[j];
    }
  }
  
  #pragma HLS LOOP_TRIPCOUNT min=80 max=80
  #pragma HLS UNROLL factor=1
  for (i = 0; i < 80; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=80 max=80
    #pragma HLS UNROLL factor=1
    for (j = i; j < 80; j++) {
      cov[i][j] = 0.0;
      #pragma HLS LOOP_TRIPCOUNT min=100 max=100
      #pragma HLS UNROLL factor=1
      for (k = 0; k < 100; k++) {
        cov[i][j] += data[k][i] * data[k][j];
      }
      cov[i][j] /= float_n - 1.0;
      cov[j][i] = cov[i][j];
    }
  }
}