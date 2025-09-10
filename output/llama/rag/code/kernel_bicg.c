void kernel_bicg(int m,int n,float A[124][116],float s[116],float q[124],float p[116],float r[124])
{
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=s offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=q offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=p offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=r offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axli port=m
  #pragma HLS INTERFACE s_axli port=n
  #pragma HLS INTERFACE s_axli port=return

  #pragma HLS DATAFLOW

  int i;
  int j;

  #pragma HLS LOOP_FLATTEN off

  #pragma HLS LOOP_TRIPCOUNT min=116 max=116

  for (i = 0; i < 116; i++) {
    s[i] = ((float )0);
  }

  #pragma HLS LOOP_FLATTEN off

  #pragma HLS LOOP_TRIPCOUNT min=124 max=124

  for (i = 0; i < 124; i++) {
    q[i] = 0.0;

    #pragma HLS LOOP_FLATTEN off

    #pragma HLS LOOP_TRIPCOUNT min=116 max=116

    for (j = 0; j < 116; j++) {
      s[j] = s[j] + r[i] * A[i][j];
      q[i] = q[i] + A[i][j] * p[j];
    }
  }
}