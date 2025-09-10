void kernel_jacobi_1d(int tsteps, int n, float A[120], float B[120]) {
    int t;
    int i;

    #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
    #pragma HLS INTERFACE s_axilite port=tsteps bundle=control
    #pragma HLS INTERFACE s_axilite port=n bundle=control
    #pragma HLS INTERFACE s_axilite port=A bundle=control
    #pragma HLS INTERFACE s_axilite port=B bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    // Optional: Partition arrays to improve parallel access
    #pragma HLS ARRAY_PARTITION variable=A cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=B cyclic factor=8 dim=1

    for (t = 0; t < 40; t++) {
        // Pipeline the first loop with II=1
        #pragma HLS PIPELINE II=1
        #pragma HLS LOOP_TRIPCOUNT min=118 max=118
        for (i = 1; i < 120 - 1; i++) {
            B[i] = 0.33333f * (A[i - 1] + A[i] + A[i + 1]);
        }

        // Pipeline the second loop with II=1
        #pragma HLS PIPELINE II=1
        #pragma HLS LOOP_TRIPCOUNT min=118 max=118
        for (i = 1; i < 120 - 1; i++) {
            A[i] = 0.33333f * (B[i - 1] + B[i] + B[i + 1]);
        }
    }
}