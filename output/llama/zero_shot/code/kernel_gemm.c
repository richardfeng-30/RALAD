void kernel_gemm(int ni,int nj,int nk,float alpha,float beta,float C[60][70],float A[60][80],float B[80][70])
{
  #pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axli offset=slave bundle=c_axi
  #pragma HLS INTERFACE s_axli offset=return bundle=c_axi

  int i;
  int j;
  int k;
//BLAS PARAMS
//TRANSA = 'N'
//TRANSB = 'N'
// => Form C := alpha*A*B + beta*C,
//A is NIxNK
//B is NKxNJ
//C is NIxNJ
{
    
    #pragma HLS LOOP_TRIPCOUNT min=1 max=60 avg=30
    for (i = 0; i < 60; i++) {
      
      #pragma HLS LOOP_TRIPCOUNT min=1 max=70 avg=35
      for (j = 0; j < 70; j++) {
        C[i][j] *= beta;
      }
      
      
      
      #pragma HLS LOOP_TRIPCOUNT min=1 max=80 avg=40
      for (k = 0; k < 80; k++) {
        
        #pragma HLS LOOP_TRIPCOUNT min=1 max=70 avg=35
        for (j = 0; j < 70; j++) {
          C[i][j] += alpha * A[i][k] * B[k][j];
        }
      }
    }
  }
}