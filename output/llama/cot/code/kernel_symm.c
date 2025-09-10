void kernel_symm(int m,int n,float alpha,float beta,float C[60][80],float A[60][60],float B[60][80])
{
  float temp2;
  int i;
  int j;
  int k;
  
  #pragma HLS INTERFACE m_axi port=A offset=slave
  #pragma HLS INTERFACE m_axi port=B offset=slave
  #pragma HLS INTERFACE m_axi port=C offset=slave
  #pragma HLS INTERFACE s_axli port=m offset=slave
  #pragma HLS INTERFACE s_axli port=n offset=slave
  #pragma HLS INTERFACE s_axli port=alpha offset=slave
  #pragma HLS INTERFACE s_axli port=beta offset=slave
  
  #pragma HLS DATAFLOW
  
  #pragma HLS pipeline II=1
  for (i = 0; i < 60; i++) {
    #pragma HLS pipeline II=1
    for (j = 0; j < 80; j++) {
      float temp = 0;
      for (k = 0; k < i; k++) {
        #pragma HLS cache
        temp += B[k][j] * A[i][k];
      }
      float C_val = beta * C[i][j];
      C_val += alpha * B[i][j] * A[i][i];
      C_val += alpha * temp;
      C[i][j] = C_val;
    }
  }
}