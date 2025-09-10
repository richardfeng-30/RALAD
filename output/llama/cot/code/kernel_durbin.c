void kernel_durbin(int n, float r[120], float y[120])
{
  #pragma HLS INTERFACE m_axi port=r[0] depth=120
  #pragma HLS INTERFACE m_axi port=y[0] depth=120
  #pragma HLS INTERFACE m_axi port=z[0] depth=120

  float z[120];
  float alpha;
  float beta;
  float sum;
  int i;
  int k;

  #pragma HLS DATAFLOW

  // Initialize y[0] and beta
  y[0] = -r[0];
  beta = 1.0;
  alpha = -r[0];

  #pragma HLS LOOP_PIPELINE II=120

  for (k = 1; k < 120; k++) {
    #pragma HLS UNROLL factor=4

    beta = (((float )1) - alpha * alpha) * beta;
    sum = 0.0;
    #pragma HLS LOOP_PIVOT

    for (i = 0; i < k; i++) {
      sum += r[k - i - 1] * y[i];
    }

    alpha = -(r[k] + sum) / beta;
    #pragma HLS LOOP_PIVOT

    for (i = 0; i < k; i++) {
      z[i] = y[i] + alpha * y[k - i - 1];
    }

    #pragma HLS LOOP_PIVOT

    for (i = 0; i < k; i++) {
      y[i] = z[i];
    }

    y[k] = alpha;
  }
}