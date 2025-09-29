```c
void kernel_atax(int m,int n,float A[116][124],float x[124],float y[124],float tmp[116])
{
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=tmp offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=m bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  int i;
  int j;
{
    #pragma HLS DATAFLOW

    for (i = 0; i < 124; i++) {
    #pragma HLS PIPELINE II=1
      y[i] = ((float )0);
    }

    for (i = 0; i < 116; i++) {
    #pragma HLS PIPELINE II=1
      tmp[i] = 0.0;

      for (j = 0; j < 124; j++) {
      #pragma HLS UNROLL factor=4
        tmp[i] = tmp[i] + A[i][j] * x[j];
      }

      for (j = 0; j < 124; j++) {
      #pragma HLS UNROLL factor=4
        y[j] = y[j] + A[i][j] * tmp[i];
      }
    }
  }
}
```