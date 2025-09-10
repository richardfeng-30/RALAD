void kernel_covariance(int m,int n,float float_n,float data[100][80],float cov[80][80],float mean[80])
{
  #pragma HLS INTERFACE m_axi port=data[0]
  #pragma HLS INTERFACE m_axi port=cov[0]
  #pragma HLS INTERFACE m_axi port=mean[0]
  
  int i;
  int j;
  int k;
  
  #pragma HLS dataflow
  
  // Calculate mean
  #pragma HLS pipeline II=1
  for (j = 0; j < 80; j++) {
    mean[j] = 0.0;
    #pragma HLS loop_tripcount 100
    #pragma HLS pipeline II=1
    for (i = 0; i < 100; i++) {
      mean[j] += data[i][j];
    }
    mean[j] /= float_n;
  }
  
  // Subtract mean from data
  #pragma HLS pipeline II=1
  for (i = 0; i < 100; i++) {
    #pragma HLS loop_tripcount 80
    for (j = 0; j < 80; j++) {
      data[i][j] -= mean[j];
    }
  }
  
  // Calculate covariance
  #pragma HLS pipeline II=1
  for (i = 0; i < 80; i++) {
    #pragma HLS loop_tripcount 80
    for (j = i; j < 80; j++) {
      cov[i][j] = 0.0;
      #pragma HLS pipeline II=1
      #pragma HLS loop_tripcount 100
      for (k = 0; k < 100; k++) {
        cov[i][j] += data[k][i] * data[k][j];
      }
      cov[i][j] /= float_n - 1.0;
      cov[j][i] = cov[i][j];
    }
  }
}