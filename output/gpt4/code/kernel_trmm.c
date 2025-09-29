```c
void kernel_trmm(int m,int n,float alpha,float A[60][60],float B[60][80])
{
#pragma HLS INLINE off
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem0 depth=3600
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem1 depth=4800
#pragma HLS INTERFACE s_axilite port=A bundle=control
#pragma HLS INTERFACE s_axilite port=B bundle=control
#pragma HLS INTERFACE s_axilite port=m bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=alpha bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    int i;
    int j;
    int k;

#pragma HLS ARRAY_PARTITION variable=A complete dim=2
#pragma HLS ARRAY_PARTITION variable=B cyclic factor=8 dim=2

    for (i = 0; i < 60; i++) {
#pragma HLS PIPELINE II=1
        for (j = 0; j < 80; j++) {
#pragma HLS UNROLL factor=8
            for (k = i + 1; k < 60; k++) {
                B[i][j] += A[k][i] * B[k][j];
            }
            B[i][j] = alpha * B[i][j];
        }
    }
}
```