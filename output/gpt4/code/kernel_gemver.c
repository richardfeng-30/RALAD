```c
void kernel_gemver(int n,float alpha,float beta,float A[120][120],float u1[120],float v1[120],float u2[120],float v2[120],float w[120],float x[120],float y[120],float z[120])
{
#pragma HLS INTERFACE m_axi port=A  offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=u1 offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=v1 offset=slave bundle=gmem2
#pragma HLS INTERFACE m_axi port=u2 offset=slave bundle=gmem3
#pragma HLS INTERFACE m_axi port=v2 offset=slave bundle=gmem4
#pragma HLS INTERFACE m_axi port=w  offset=slave bundle=gmem5
#pragma HLS INTERFACE m_axi port=x  offset=slave bundle=gmem6
#pragma HLS INTERFACE m_axi port=y  offset=slave bundle=gmem7
#pragma HLS INTERFACE m_axi port=z  offset=slave bundle=gmem8
#pragma HLS INTERFACE s_axilite port=n
#pragma HLS INTERFACE s_axilite port=alpha
#pragma HLS INTERFACE s_axilite port=beta
#pragma HLS INTERFACE s_axilite port=return

    int i;
    int j;

    // Update A
    for (i = 0; i < 120; i++) {
#pragma HLS PIPELINE II=1
        for (j = 0; j < 120; j++) {
#pragma HLS UNROLL factor=4
            A[i][j] = A[i][j] + u1[i] * v1[j] + u2[i] * v2[j];
        }
    }

    // x = x + beta * A^T * y
    for (i = 0; i < 120; i++) {
#pragma HLS PIPELINE II=1
        for (j = 0; j < 120; j++) {
#pragma HLS UNROLL factor=4
            x[i] = x[i] + beta * A[j][i] * y[j];
        }
    }

    // x = x + z
    for (i = 0; i < 120; i++) {
#pragma HLS PIPELINE II=1
        x[i] = x[i] + z[i];
    }

    // w = w + alpha * A * x
    for (i = 0; i < 120; i++) {
#pragma HLS PIPELINE II=1
        for (j = 0; j < 120; j++) {
#pragma HLS UNROLL factor=4
            w[i] = w[i] + alpha * A[i][j] * x[j];
        }
    }
}
```