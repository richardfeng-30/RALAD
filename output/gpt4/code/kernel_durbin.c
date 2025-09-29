```c
void kernel_durbin(int n,float r[120],float y[120])
{
#pragma HLS INLINE off
  float z[120];
  float alpha;
  float beta;
  float sum;
  int i;
  int k;
#pragma HLS ARRAY_PARTITION variable=z complete dim=1
#pragma HLS ARRAY_PARTITION variable=y complete dim=1
#pragma HLS ARRAY_PARTITION variable=r complete dim=1
  {
    y[0] = -r[0];
    beta = 1.0;
    alpha = -r[0];

    for (k = 1; k < 120; k++) {
#pragma HLS PIPELINE II=1
      beta = (((float )1) - alpha * alpha) * beta;
      sum = 0.0;
      for (i = 0; i < k; i++) {
#pragma HLS UNROLL
        sum += r[k - i - 1] * y[i];
      }
      alpha = -(r[k] + sum) / beta;
      for (i = 0; i < k; i++) {
#pragma HLS UNROLL
        z[i] = y[i] + alpha * y[k - i - 1];
      }
      for (i = 0; i < k; i++) {
#pragma HLS UNROLL
        y[i] = z[i];
      }
      y[k] = alpha;
    }
  }
}
```