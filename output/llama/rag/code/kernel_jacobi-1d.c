void kernel_jacobi_1d(int tsteps, int n, float A[120], float B[120]) {
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axli port=tsteps
  #pragma HLS INTERFACE s_axli port=n
  #pragma HLS INTERFACE s_axlo port=A
  #pragma HLS INTERFACE s_axlo port=B
  #pragma HLS INTERFACE s_axlo port=s_ax
  #pragma HLS DATAFLOW

  int t;
  int i;

  #pragma HLS DATAFLOW
  ShiftAccumLoop:
  for (t = 0; t < 40; t++) {
    #pragma HLS LOOP_TRIPCOUNT min=40 max=40
    #pragma HLS UNROLL factor=1
    for (i = 1; i < 120 - 1; i++) {
      #pragma HLS LOOP_TRIPCOUNT min=119 max=119
      #pragma HLS UNROLL factor=1
      B[i] = 0.33333 * (A[i - 1] + A[i] + A[i + 1]);
    }
    #pragma HLS DATAFLOW
    ShiftAccumLoop2:
    for (i = 1; i < 120 - 1; i++) {
      #pragma HLS LOOP_TRIPCOUNT min=119 max=119
      #pragma HLS UNROLL factor=1
      A[i] = 0.33333 * (B[i - 1] + B[i] + B[i + 1]);
    }
  }
}