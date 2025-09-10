void kernel_trisolv(int n,float L[120][120],float x[120],float b[120])
{
  #pragma HLS INTERFACE m_axi port=L offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=b offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axli port=n
  #pragma HLS INTERFACE s_axli port=return

  int i;
  int j;
  #pragma HLS DATAFLOW

  #pragma HLS LOOP_FLATTEN variable=i
  #pragma HLS LOOP_FLATTEN variable=j
  for (i = 0; i < 120; i++) {
    x[i] = b[i];
    #pragma HLS LOOP_FLATTEN variable=j
    for (j = 0; j < i; j++) {
      x[i] -= L[i][j] * x[j];
    }
    x[i] = x[i] / L[i][i];
  }
}