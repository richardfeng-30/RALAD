void kernel_2mm(int ni,int nj,int nk,int nl,float alpha,float beta,float tmp[40][50],float A[40][70],float B[70][50],float C[50][80],float D[40][80])
{
  #pragma HLS INTERFACE m_axi port=tmp offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=D offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axilite port=return bundle=control
  #pragma HLS INTERFACE s_axilite port=alpha bundle=control
  #pragma HLS INTERFACE s_axilite port=beta bundle=control
  #pragma HLS INTERFACE s_axilite port=ni bundle=control
  #pragma HLS INTERFACE s_axilite port=nj bundle=control
  #pragma HLS INTERFACE s_axilite port=nk bundle=control
  #pragma HLS INTERFACE s_axilite port=nl bundle=control

  #pragma HLS DATAFLOW

  //2 Matrix Multiplications
  #pragma HLS DATAFLOW
  for (int i = 0; i < ni; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=40 max=40
    for (int j = 0; j < nj; j++) {
      #pragma HLS LOOP_TRIPCOUNT min=50 max=50
      tmp[i][j] = 0.0;
      
      #pragma HLS DATAFLOW
      for (int k = 0; k < nk; ++k) {
        #pragma HLS LOOP_TRIPCOUNT min=70 max=70
        tmp[i][j] += alpha * A[i][k] * B[k][j];
      }
    }
  }
  
  #pragma HLS DATAFLOW
  for (int i = 0; i < ni; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=40 max=40
    for (int j = 0; j < nl; j++) {
      #pragma HLS LOOP_TRIPCOUNT min=80 max=80
      D[i][j] *= beta;
      
      #pragma HLS DATAFLOW
      for (int k = 0; k < nj; ++k) {
        #pragma HLS LOOP_TRIPCOUNT min=50 max=50
        D[i][j] += tmp[i][k] * C[k][j];
      }
    }
  }
}