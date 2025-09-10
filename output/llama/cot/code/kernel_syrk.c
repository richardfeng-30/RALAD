void kernel_syrk(int n,int m,float alpha,float beta,float C[80][80],float A[80][60])
{
  int i;
  int j;
  int k;
//BLAS PARAMS
//TRANS = 'N'
//UPLO  = 'L'
// =>  Form  C := alpha*A*A**T + beta*C.
//A is NxM
//C is NxN

  #pragma HLS dataflow
  #pragma HLS pipeline II=1
  
  for (i = 0; i < 80; i++) {
    #pragma HLS loop_tripcount min=1 max=80
    #pragma HLS pipeline II=1
    
    for (j = 0; j <= i; j++) {
      #pragma HLS unroll
      C[i][j] *= beta;
    }
    
    for (k = 0; k < 60; k++) {
      #pragma HLS loop_tripcount min=1 max=60
      #pragma HLS pipeline II=1
      
      for (j = 0; j <= i; j++) {
        #pragma HLS unroll
        C[i][j] += alpha * A[i][k] * A[j][k];
      }
    }
  }
}