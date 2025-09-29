```c
void kernel_trisolv(int n,float L[120][120],float x[120],float b[120])
{
#pragma HLS INLINE off
#pragma HLS ARRAY_PARTITION variable=L complete dim=2
#pragma HLS ARRAY_PARTITION variable=x complete dim=1
#pragma HLS ARRAY_PARTITION variable=b complete dim=1
  int i;
  int j;
  for (i = 0; i < 120; i++) {
#pragma HLS PIPELINE II=1
    x[i] = b[i];
    for (j = 0; j < i; j++) {
#pragma HLS UNROLL
      x[i] -= L[i][j] * x[j];
    }
    x[i] = x[i] / L[i][i];
  }
}
```