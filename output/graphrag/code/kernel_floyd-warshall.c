void kernel_floyd_warshall(int n, int path[180][180])
{
    int i, j, k;

    // Partition the second dimension of path array cyclically to enable parallel access
    #pragma HLS array_partition variable=path cyclic factor=4 dim=2

    // Flatten the outer two loops to reduce loop control overhead
    #pragma HLS loop_flatten

    for (k = 0; k < 180; k++) {
        for (i = 0; i < 180; i++) {
            // Pipeline the innermost loop to improve throughput
            #pragma HLS pipeline II=1
            for (j = 0; j < 180; j++) {
                int via_k = path[i][k] + path[k][j];
                if (path[i][j] > via_k) {
                    path[i][j] = via_k;
                }
            }
        }
    }
}