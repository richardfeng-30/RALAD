void kernel_durbin(int n, float r[120], float y[120]) {
#pragma HLS INTERFACE m_axi port = r bundle = aximm depth = 120
#pragma HLS INTERFACE m_axi port = y bundle = aximm depth = 120

#pragma HLS cache port = r lines = 1 depth = 128
#pragma HLS cache port = y lines = 1 depth = 128

  float z[120];
  float alpha;
  float beta;
  float sum;
  int i;
  int k;

  y[0] = -r[0];
  beta = 1.0;
  alpha = -r[0];

  #pragma HLS loop_tripcount min = 1 max = 119
  for (k = 1; k < 120; k++) {
    beta = (((float )1) - alpha * alpha) * beta;
    #pragma HLS loop_tripcount min = 1 max = k
    sum = 0.0;
    for (i = 0; i < k; i++) {
      sum += r[k - i - 1] * y[i];
    }
    alpha = -(r[k] + sum) / beta;
    #pragma HLS loop_tripcount min = 1 max = k
    for (i = 0; i < k; i++) {
      z[i] = y[i] + alpha * y[k - i - 1];
    }
    #pragma HLS loop_tripcount min = 1 max = k
    for (i = 0; i < k; i++) {
      y[i] = z[i];
    }
    y[k] = alpha;
  }
}