void kernel_gesummv(int n, float alpha, float beta, float A[90][90], float B[90][90], float tmp[90], float x[90], float y[90]) {
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=tmp offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem

  #pragma HLS INTERFACE s_axilite port=n
  #pragma HLS INTERFACE s_axilite port=alpha
  #pragma HLS INTERFACE s_axilite port=beta

  int i;
  int j;
  #pragma HLS loop_tripcount min=1 max=90
  for (i = 0; i < 90; i++) {
    #pragma HLS loop_tripcount min=1 max=90
    for (j = 0; j < 90; j++) {
      tmp[i] = A[i][j] * x[j] + tmp[i];
      y[i] = B[i][j] * x[j] + y[i];
    }
    y[i] = alpha * tmp[i] + beta * y[i];
  }
}