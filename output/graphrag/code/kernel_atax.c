void kernel_atax(int m, int n, float A[116][124], float x[124], float y[124], float tmp[116])
{
  int i;
  int j;

  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem0
  #pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem1
  #pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem2
  #pragma HLS INTERFACE m_axi port=tmp offset=slave bundle=gmem3
  #pragma HLS INTERFACE s_axilite port=m bundle=control
  #pragma HLS INTERFACE s_axilite port=n bundle=control
  #pragma HLS INTERFACE s_axilite port=A bundle=control
  #pragma HLS INTERFACE s_axilite port=x bundle=control
  #pragma HLS INTERFACE s_axilite port=y bundle=control
  #pragma HLS INTERFACE s_axilite port=tmp bundle=control
  #pragma HLS INTERFACE s_axilite port=return bundle=control

  // Partition tmp array completely for parallel access
  #pragma HLS ARRAY_PARTITION variable=tmp complete dim=1
  // Partition x and y partially to allow parallel access
  #pragma HLS ARRAY_PARTITION variable=x cyclic factor=8 dim=1
  #pragma HLS ARRAY_PARTITION variable=y cyclic factor=8 dim=1
  // Partition A partially on the second dimension for parallel access
  #pragma HLS ARRAY_PARTITION variable=A cyclic factor=8 dim=2

  // Initialize y array
  init_y: for (i = 0; i < 124; i++) {
    #pragma HLS PIPELINE II=1
    y[i] = 0.0f;
  }

  atax_outer: for (i = 0; i < 116; i++) {
    #pragma HLS PIPELINE II=1
    tmp[i] = 0.0f;

    atax_inner1: for (j = 0; j < 124; j++) {
      #pragma HLS UNROLL factor=4
      tmp[i] += A[i][j] * x[j];
    }

    atax_inner2: for (j = 0; j < 124; j++) {
      #pragma HLS PIPELINE II=1
      y[j] += A[i][j] * tmp[i];
    }
  }
}