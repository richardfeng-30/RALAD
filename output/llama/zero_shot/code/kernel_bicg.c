void kernel_bicg(int m,int n,float A[124][116],float s[116],float q[124],float p[116],float r[124])
{
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=s offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=q offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=p offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=r offset=slave bundle=gmem

  #pragma HLS INTERFACE s_axilite port=m
  #pragma HLS INTERFACE s_axilite port=n
  #pragma HLS INTERFACE s_axilite port=A
  #pragma HLS INTERFACE s_axilite port=s
  #pragma HLS INTERFACE s_axilite port=q
  #pragma HLS INTERFACE s_axilite port=p
  #pragma HLS INTERFACE s_axilite port=r
  #pragma HLS INTERFACE s_axilite port=return

  int i;
  int j;
  {
    
    #pragma HLS LOOP_FLATTEN
    for (i = 0; i < 116; i++) {
      s[i] = ((float )0);
    }
    
    
    
    #pragma HLS LOOP_FLATTEN
    for (i = 0; i < 124; i++) {
      q[i] = 0.0;
      
      #pragma HLS LOOP_FLATTEN
      for (j = 0; j < 116; j++) {
        s[j] = s[j] + r[i] * A[i][j];
        q[i] = q[i] + A[i][j] * p[j];
      }
    }
  }
}