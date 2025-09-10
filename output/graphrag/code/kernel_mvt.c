void kernel_mvt(int n, float x1[120], float x2[120], float y_1[120], float y_2[120], float A[120][120])
{
    int i, j;

    // Partition arrays to enable parallel access and reduce memory bottlenecks
    #pragma HLS array_partition variable=A complete dim=2
    #pragma HLS array_partition variable=y_1 complete
    #pragma HLS array_partition variable=y_2 complete

    // First matrix-vector multiplication loop
    for (i = 0; i < 120; i++) {
        #pragma HLS pipeline II=1
        float sum1 = 0;
        for (j = 0; j < 120; j++) {
            #pragma HLS unroll factor=4
            sum1 += A[i][j] * y_1[j];
        }
        x1[i] += sum1;
    }

    // Second matrix-vector multiplication loop
    for (i = 0; i < 120; i++) {
        #pragma HLS pipeline II=1
        float sum2 = 0;
        for (j = 0; j < 120; j++) {
            #pragma HLS unroll factor=4
            sum2 += A[j][i] * y_2[j];
        }
        x2[i] += sum2;
    }
}