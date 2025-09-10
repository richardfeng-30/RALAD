void kernel_gesummv(int n, float alpha, float beta, float A[90][90], float B[90][90], float tmp[90], float x[90], float y[90])
{
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=tmp offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=alpha bundle=control
#pragma HLS INTERFACE s_axilite port=beta bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  // Partition arrays to enable parallel access
#pragma HLS ARRAY_PARTITION variable=A complete dim=2
#pragma HLS ARRAY_PARTITION variable=B complete dim=2
#pragma HLS ARRAY_PARTITION variable=x complete dim=1

  int i, j;

  // Pipeline outer loop to improve throughput
  for (i = 0; i < 90; i++) {
#pragma HLS PIPELINE II=1

    tmp[i] = 0.0;
    y[i] = 0.0;

    // Pipeline and partially unroll inner loop for parallelism
    for (j = 0; j < 90; j++) {
#pragma HLS PIPELINE II=1
#pragma HLS UNROLL factor=4

      tmp[i] += A[i][j] * x[j];
      y[i] += B[i][j] * x[j];
    }

    y[i] = alpha * tmp[i] + beta * y[i];
  }
}