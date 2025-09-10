void kernel_atax(int m,int n,float A[116][124],float x[124],float y[124],float tmp[116])
{
  #pragma HLS INTERFACE m_axis port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axis port=x offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axis port=y offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axis port=tmp offset=slave bundle=gmem

  #pragma HLS INTERFACE s_axilite port=m
  #pragma HLS INTERFACE s_axilite port=n
  #pragma HLS INTERFACE s_axilite port=return

  #pragma HLS DATAFLOW

  int i;
  int j;

  #pragma HLS DATA_REORDER variable=y
  #pragma HLS DATA_REORDER variable=tmp

  for (i = 0; i < 124; i++) {
    y[i] = ((float )0);
  }

  #pragma HLS LOOP_TRIPCOUNT max=124
  #pragma HLS UNROLL factor=4

  for (i = 0; i < 116; i++) {
    tmp[i] = 0.0;

    #pragma HLS LOOP_TRIPCOUNT max=124
    #pragma HLS UNROLL factor=4

    for (j = 0; j < 124; j++) {
      tmp[i] = tmp[i] + A[i][j] * x[j];
    }

    #pragma HLS LOOP_TRIPCOUNT max=124
    #pragma HLS UNROLL factor=4

    for (j = 0; j < 124; j++) {
      y[j] = y[j] + A[i][j] * tmp[i];
    }
  }
}