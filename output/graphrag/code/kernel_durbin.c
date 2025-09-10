void kernel_durbin(int n, float r[120], float y[120]) {
#pragma HLS INTERFACE m_axi port=r offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=r bundle=control
#pragma HLS INTERFACE s_axilite port=y bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    float z[120];
#pragma HLS ARRAY_PARTITION variable=y complete dim=1
#pragma HLS ARRAY_PARTITION variable=z complete dim=1

    float alpha;
    float beta;
    float sum;
    int i;
    int k;

    y[0] = -r[0];
    beta = 1.0;
    alpha = -r[0];

    // Pipeline the outer loop to improve throughput
    for (k = 1; k < 120; k++) {
#pragma HLS PIPELINE II=1

        beta = (1.0f - alpha * alpha) * beta;
        sum = 0.0f;

        // Partially unroll the inner loop to balance resource usage and performance
        for (i = 0; i < k; i++) {
#pragma HLS UNROLL factor=4
            sum += r[k - i - 1] * y[i];
        }

        alpha = -(r[k] + sum) / beta;

        for (i = 0; i < k; i++) {
#pragma HLS UNROLL factor=4
            z[i] = y[i] + alpha * y[k - i - 1];
        }

        for (i = 0; i < k; i++) {
#pragma HLS UNROLL factor=4
            y[i] = z[i];
        }

        y[k] = alpha;
    }
}