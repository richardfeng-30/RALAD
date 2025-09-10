void kernel_trisolv(int n, float L[120][120], float x[120], float b[120]) {
    int i, j;

    // Partition the L matrix to enable parallel access to columns
    #pragma HLS array_partition variable=L cyclic factor=8 dim=2
    // Partition the x vector to enable parallel access
    #pragma HLS array_partition variable=x cyclic factor=8

    // Specify loop tripcounts for better optimization estimation
    #pragma HLS loop_tripcount min=120 max=120

    for (i = 0; i < 120; i++) {
        #pragma HLS pipeline II=1
        x[i] = b[i];

        // Inner loop has data dependency, so unroll partially to balance resource usage
        #pragma HLS unroll factor=4
        for (j = 0; j < i; j++) {
            x[i] -= L[i][j] * x[j];
        }

        x[i] = x[i] / L[i][i];
    }
}