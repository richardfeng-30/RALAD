void kernel_doitgen(int nr,int nq,int np,float A[25][20][30],float C4[30][30],float sum[30])
{
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=C4 offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=sum offset=slave bundle=gmem

  #pragma HLS INTERFACE s_axilite port=nr
  #pragma HLS INTERFACE s_axilite port=nq
  #pragma HLS INTERFACE s_axilite port=np

  #pragma HLS INTERFACE s_axilite port=return

  int r;
  int q;
  int p;
  int s;
{
    
    
    #pragma HLS LOOP_TRIPCOUNT min=25 max=25 avg=25
    for (r = 0; r < 25; r++) {
      
      
      #pragma HLS LOOP_TRIPCOUNT min=20 max=20 avg=20
      for (q = 0; q < 20; q++) {
        
        
        #pragma HLS LOOP_TRIPCOUNT min=30 max=30 avg=30
        for (p = 0; p < 30; p++) {
          sum[p] = 0.0;
          #pragma HLS LOOP_TRIPCOUNT min=30 max=30 avg=30
          for (s = 0; s < 30; s++) {
            sum[p] += A[r][q][s] * C4[s][p];
          }
        }
        #pragma HLS LOOP_TRIPCOUNT min=30 max=30 avg=30
        for (p = 0; p < 30; p++) {
          A[r][q][p] = sum[p];
        }
      }
    }
  }
}