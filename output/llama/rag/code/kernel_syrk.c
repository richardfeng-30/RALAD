void kernel_syrk(int n,int m,float alpha,float beta,float C[80][80],float A[80][60])
{
  #pragma HLS INTERFACE m_axi port=C offset=slave
  #pragma HLS INTERFACE m_axi port=A offset=slave
  #pragma HLS INTERFACE s_axli port=n offset=slave
  #pragma HLS INTERFACE s_axli port=m offset=slave
  #pragma HLS INTERFACE s_axli port=alpha offset=slave
  #pragma HLS INTERFACE s_axli port=beta offset=slave
  
  int i;
  int j;
  int k;
  #pragma HLS loop_tripcount min=1 max=80
  #pragma HLS loop_tripcount min=1 max=80
  
  //BLAS PARAMS
  //TRANS = 'N'
  //UPLO  = 'L'
  // =>  Form  C := alpha*A*A**T + beta*C.
  //A is NxM
  //C is NxN
  
  {
    #pragma HLS loop_unroll factor=4
    for (i = 0; i < 80; i++) {
      #pragma HLS loop_tripcount min=1 max=80
      for (j = 0; j <= i; j++) {
        C[i][j] *= beta;
      }
      
      
      
      #pragma HLS loop_unroll factor=4
      for (k = 0; k < 60; k++) {
        for (j = 0; j <= i; j++) {
          C[i][j] += alpha * A[i][k] * A[j][k];
        }
      }
    }
  }
}