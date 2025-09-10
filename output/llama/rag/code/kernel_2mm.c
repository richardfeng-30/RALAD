void kernel_2mm(int ni,int nj,int nk,int nl,float alpha,float beta,float tmp[40][50],float A[40][70],float B[70][50],float C[50][80],float D[40][80])
{
  #pragma HLS INTERFACE m_axi port=tmp offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=D offset=slave bundle=gmem

  #pragma HLS INTERFACE s_axilite port=ni
  #pragma HLS INTERFACE s_axilite port=nj
  #pragma HLS INTERFACE s_axilite port=nk
  #pragma HLS INTERFACE s_axilite port=nl
  #pragma HLS INTERFACE s_axilite port=alpha
  #pragma HLS INTERFACE s_axilite port=beta

  #pragma HLS DATAFLOW

  #pragma HLS LOOP_TRIPCOUNT min=40 max=40
  #pragma HLS LOOP_TRIPCOUNT min=50 max=50
  #pragma HLS LOOP_TRIPCOUNT min=70 max=70
  #pragma HLS LOOP_TRIPCOUNT min=50 max=50
  #pragma HLS LOOP_TRIPCOUNT min=80 max=80

  int i;
  int j;
  int k;

  #pragma HLS PIPELINE II=1
  for (i = 0; i < 40; i++) {
    #pragma HLS LOOP_UNROLL
    for (j = 0; j < 50; j++) {
      tmp[i][j] = 0.0;
      #pragma HLS UNROLL
      for (k = 0; k < 70; ++k) {
        tmp[i][j] += alpha * A[i][k] * B[k][j];
      }
    }
  }

  #pragma HLS PIPELINE II=1
  for (i = 0; i < 40; i++) {
    #pragma HLS LOOP_UNROLL
    for (j = 0; j < 80; j++) {
      D[i][j] *= beta;
      #pragma HLS UNROLL
      for (k = 0; k < 50; ++k) {
        D[i][j] += tmp[i][k] * C[k][j];
      }
    }
  }
}