```c
void kernel_gesummv(int n,float alpha,float beta,float A[90][90],float B[90][90],float tmp[90],float x[90],float y[90])
{
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=tmp offset=slave bundle=gmem2
#pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem3
#pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem4
#pragma HLS INTERFACE s_axilite port=A bundle=control
#pragma HLS INTERFACE s_axilite port=B bundle=control
#pragma HLS INTERFACE s_axilite port=tmp bundle=control
#pragma HLS INTERFACE s_axilite port=x bundle=control
#pragma HLS INTERFACE s_axilite port=y bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=alpha bundle=control
#pragma HLS INTERFACE s_axilite port=beta bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS ARRAY_PARTITION variable=A cyclic factor=10 dim=2
#pragma HLS ARRAY_PARTITION variable=B cyclic factor=10 dim=2
#pragma HLS ARRAY_PARTITION variable=x cyclic factor=10 dim=1

    int i;
    int j;
    for (i = 0; i < 90; i++) {
#pragma HLS PIPELINE II=1
      float tmp_i = 0.0;
      float y_i = 0.0;
      for (j = 0; j < 90; j++) {
#pragma HLS UNROLL factor=10
        tmp_i += A[i][j] * x[j];
        y_i += B[i][j] * x[j];
      }
      tmp[i] = tmp_i;
      y[i] = alpha * tmp_i + beta * y_i;
    }
}
```