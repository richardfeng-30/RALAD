```c
void kernel_bicg(int m,int n,float A[124][116],float s[116],float q[124],float p[116],float r[124])
{
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=s offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=q offset=slave bundle=gmem2
#pragma HLS INTERFACE m_axi port=p offset=slave bundle=gmem3
#pragma HLS INTERFACE m_axi port=r offset=slave bundle=gmem4
#pragma HLS INTERFACE s_axilite port=A bundle=control
#pragma HLS INTERFACE s_axilite port=s bundle=control
#pragma HLS INTERFACE s_axilite port=q bundle=control
#pragma HLS INTERFACE s_axilite port=p bundle=control
#pragma HLS INTERFACE s_axilite port=r bundle=control
#pragma HLS INTERFACE s_axilite port=m bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  int i;
  int j;
{
#pragma HLS DATAFLOW

    for (i = 0; i < 116; i++) {
#pragma HLS PIPELINE II=1
      s[i] = ((float )0);
    }
    
    for (i = 0; i < 124; i++) {
#pragma HLS PIPELINE II=1
      q[i] = 0.0;
      for (j = 0; j < 116; j++) {
#pragma HLS UNROLL factor=4
        s[j] = s[j] + r[i] * A[i][j];
        q[i] = q[i] + A[i][j] * p[j];
      }
    }
  }
}
```