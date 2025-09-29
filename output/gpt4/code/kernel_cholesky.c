```c
#include <math.h>

void kernel_cholesky(int n, float A[120][120])
{
#pragma HLS INLINE off
#pragma HLS ARRAY_PARTITION variable=A complete dim=2
    int i;
    int j;
    int k;
    for (i = 0; i < 120; i++) {
#pragma HLS PIPELINE II=1
        for (j = 0; j < i; j++) {
#pragma HLS UNROLL
            for (k = 0; k < j; k++) {
#pragma HLS UNROLL
                A[i][j] -= A[i][k] * A[j][k];
            }
            A[i][j] /= A[j][j];
        }
        for (k = 0; k < i; k++) {
#pragma HLS UNROLL
            A[i][i] -= A[i][k] * A[i][k];
        }
        A[i][i] = sqrt(A[i][i]);
    }
}
```