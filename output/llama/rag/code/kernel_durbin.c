void kernel_durbin(int n, float r[120], float y[120]) {
  #pragma HLS INTERFACE m_axi port=r offset=slave
  #pragma HLS INTERFACE m_axi port=y offset=slave
  #pragma HLS INTERFACE s_axilite port=n
  #pragma HLS INTERFACE s_axilite port=return

  float z[120];
  float alpha;
  float beta;
  float sum;
  int i;
  int k;

  #pragma HLS DATAFLOW
  #pragma HLS DATA_REORDER

  y[0] = -r[0];
  beta = 1.0;
  alpha = -r[0];

  #pragma HLS LOOP_FLATTEN
  for (k = 1; k < 120; k++) {
    #pragma HLS UNROLL
    beta = (((float)1) - alpha * alpha) * beta;
    sum = 0.0;
    #pragma HLS LOOP_FLATTEN
    for (i = 0; i < k; i++) {
      sum += r[k - i - 1] * y[i];
    }
    alpha = -(r[k] + sum) / beta;
    #pragma HLS LOOP_FLATTEN
    for (i = 0; i < k; i++) {
      z[i] = y[i] + alpha * y[k - i - 1];
    }
    #pragma HLS LOOP_FLATTEN
    for (i = 0; i < k; i++) {
      y[i] = z[i];
    }
    y[k] = alpha;
  }
}