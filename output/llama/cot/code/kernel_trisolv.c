void kernel_trisolv(int n, float L[120][120], float x[120], float b[120])
{
  #pragma HLS INTERFACE m_axi port=L offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=b offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axli port=n
  #pragma HLS INTERFACE s_axli port=x
  #pragma HLS INTERFACE s_axli port=b
  #pragma HLS INTERFACE s_axli port=x
  
  #pragma HLS DATAFLOW
  
  #pragma HLS LOOP_FLATTEN min_loop_count=120 loop_limit=120
  for (int i = 0; i < 120; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=120 max=120 avg=120
    x[i] = b[i];
    for (int j = 0; j < i; j++) {
      #pragma HLS LOOP_FLATTEN min_loop_count=120 loop_limit=120
      #pragma HLS LOOP_TRIPCOUNT min=120 max=120 avg=120
      x[i] -= L[i][j] * x[j];
    }
    x[i] = x[i] / L[i][i];
  }
}