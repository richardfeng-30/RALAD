```c
void kernel_lu(int n,float A[120][120])
{
#pragma HLS INLINE off
#pragma HLS ARRAY_PARTITION variable=A cyclic factor=8 dim=2
    int i;
    int j;
    int k;
    for (i = 0; i < 120; i++) {
#pragma HLS PIPELINE II=1
        for (j = 0; j < i; j++) {
#pragma HLS UNROLL factor=4
            for (k = 0; k < j; k++) {
#pragma HLS UNROLL factor=4
                A[i][j] -= A[i][k] * A[k][j];
            }
            A[i][j] /= A[j][j];
        }
        for (j = i; j < 120; j++) {
#pragma HLS UNROLL factor=4
            for (k = 0; k < i; k++) {
#pragma HLS UNROLL factor=4
                A[i][j] -= A[i][k] * A[k][j];
            }
        }
    }
}
```