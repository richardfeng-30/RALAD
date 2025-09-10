void kernel_gemver(int n, float alpha, float beta, float A[120][120], float u1[120], float v1[120], float u2[120], float v2[120], float w[120], float x[120], float y[120], float z[120]) {
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=u1 offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=v1 offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=u2 offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=v2 offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=w offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=z offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=alpha bundle=control
#pragma HLS INTERFACE s_axilite port=beta bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS array_partition variable=A complete dim=2
#pragma HLS array_partition variable=u1 complete
#pragma HLS array_partition variable=v1 complete
#pragma HLS array_partition variable=u2 complete
#pragma HLS array_partition variable=v2 complete
#pragma HLS array_partition variable=w complete
#pragma HLS array_partition variable=x complete
#pragma HLS array_partition variable=y complete
#pragma HLS array_partition variable=z complete

#pragma HLS dataflow

    int i, j;

    // Update A matrix
    for (i = 0; i < 120; i++) {
#pragma HLS pipeline II=1
        for (j = 0; j < 120; j++) {
#pragma HLS unroll factor=4
            A[i][j] = A[i][j] + u1[i] * v1[j] + u2[i] * v2[j];
        }
    }

    // Update x vector
    for (i = 0; i < 120; i++) {
        float temp = 0;
#pragma HLS pipeline II=1
        for (j = 0; j < 120; j++) {
#pragma HLS unroll factor=4
            temp += beta * A[j][i] * y[j];
        }
        x[i] = x[i] + temp + z[i];
    }

    // Update w vector
    for (i = 0; i < 120; i++) {
        float temp = 0;
#pragma HLS pipeline II=1
        for (j = 0; j < 120; j++) {
#pragma HLS unroll factor=4
            temp += alpha * A[i][j] * x[j];
        }
        w[i] = w[i] + temp;
    }
}