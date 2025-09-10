void kernel_syrk(int n, int m, float alpha, float beta, float C[80][80], float A[80][60])
{
    int i, j, k;

    // Partition the arrays to enable parallel access
    #pragma HLS ARRAY_PARTITION variable=C complete dim=2
    #pragma HLS ARRAY_PARTITION variable=A complete dim=2

    // Pipeline the outer loop to improve throughput
    for (i = 0; i < 80; i++) {
        // Pipeline the j loop with dependence pragma to allow concurrent updates
        for (j = 0; j <= i; j++) {
            #pragma HLS PIPELINE II=1
            C[i][j] *= beta;
        }

        // Pipeline the k loop to overlap iterations
        for (k = 0; k < 60; k++) {
            // Unroll the j loop partially to balance resource usage and parallelism
            for (j = 0; j <= i; j++) {
                #pragma HLS UNROLL factor=4
                #pragma HLS PIPELINE II=1
                C[i][j] += alpha * A[i][k] * A[j][k];
            }
        }
    }
}