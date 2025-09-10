void kernel_trmm(int m,int n,float alpha,float A[60][60],float B[60][80])
{
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
  #pragma HLS INTERFACE axis port=A
  #pragma HLS INTERFACE axis port=B

  #pragma HLS ARRAY RESHAPE variable=A complete dim=2
  #pragma HLS ARRAY RESHAPE variable=B complete dim=2

  int i;
  int j;
  int k;
  
  //BLAS parameters
  //SIDE   = 'L'
  //UPLO   = 'L'
  //TRANSA = 'T'
  //DIAG   = 'U'
  // => Form  B := alpha*A**T*B.
  // A is MxM
  // B is MxN

  {
    #pragma HLS LOOP_TRIPCOUNT min=1 max=60
    for (i = 0; i < 60; i++) {
      
      #pragma HLS LOOP_TRIPCOUNT min=1 max=80
      for (j = 0; j < 80; j++) {
        float sum = 0.0;
        #pragma HLS LOOP_TRIPCOUNT min=1 max=59
        for (k = i + 1; k < 60; k++) {
          sum += A[k][i] * B[k][j];
        }
        B[i][j] = alpha * (B[i][j] + sum);
      }
    }
  }
}