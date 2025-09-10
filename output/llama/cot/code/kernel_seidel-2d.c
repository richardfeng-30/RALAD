void kernel_seidel_2d(int tsteps,int n,float A[120][120])
{
  #pragma HLS INTERFACE m_axi port=A[0] depth=120
  #pragma HLS INTERFACE axis port=A[0]
  #pragma HLS INTERFACE s_axilite port=n
  #pragma HLS INTERFACE s_axilite port=tsteps
  #pragma HLS INTERFACE s_axilite port=t
  #pragma HLS INTERFACE s_axilite port=i
  #pragma HLS INTERFACE s_axilite port=j
  
  int t;
  int i;
  int j;
  {
    for (t = 0; t <= 40 - 1; t++) {
      #pragma HLS LOOP_TRIPCOUNT min=1 max=40 avg=20
      #pragma HLS PIPELINE II=1
      
      for (i = 1; i <= 120 - 2; i++) {
        #pragma HLS LOOP_TRIPCOUNT min=1 max=119 avg=60
        #pragma HLS PIPELINE II=1
        
        for (j = 1; j <= 120 - 2; j++) {
          #pragma HLS LOOP_TRIPCOUNT min=1 max=119 avg=60
          #pragma HLS PIPELINE II=1
          A[i][j] = (A[i - 1][j - 1] + A[i - 1][j] + A[i - 1][j + 1] + A[i][j - 1] + A[i][j] + A[i][j + 1] + A[i + 1][j - 1] + A[i + 1][j] + A[i + 1][j + 1]) / 9.0;
        }
      }
    }
  }
}