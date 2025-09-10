void kernel_doitgen(int nr, int nq, int np, float A[25][20][30], float C4[30][30], float sum[30])
{
  #pragma HLS interface m_axi port=A[0] dim=2
  #pragma HLS interface m_axi port=C4[0] dim=2
  #pragma HLS interface m_axi port=sum[0] dim=1
  
  int r;
  int q;
  int p;
  int s;
  
  #pragma HLS dataflow
  #pragma HLS pipeline II=25
  
  for (r = 0; r < 25; r++) {
    #pragma HLS loop_tripcount 25
    #pragma HLS pipeline II=20
    
    for (q = 0; q < 20; q++) {
      #pragma HLS loop_tripcount 20
      #pragma HLS pipeline II=30
      
      for (p = 0; p < 30; p++) {
        sum[p] = 0.0;
        #pragma HLS unroll
        for (s = 0; s < 30; s++) {
          sum[p] += A[r][q][s] * C4[s][p];
        }
      }
      #pragma HLS pipeline II=30
      
      for (p = 0; p < 30; p++) {
        A[r][q][p] = sum[p];
      }
    }
  }
}