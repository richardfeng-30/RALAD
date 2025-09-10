void kernel_doitgen(int nr, int nq, int np, float A[25][20][30], float C4[30][30], float sum[30])
{
    int r;
    int q;
    int p;
    int s;

    // Partition sum array completely to allow parallel access
    #pragma HLS ARRAY_PARTITION variable=sum complete dim=1

    for (r = 0; r < 25; r++) {
        for (q = 0; q < 20; q++) {

            // Pipeline the loop over p to improve throughput
            for (p = 0; p < 30; p++) {
                sum[p] = 0.0;
            }

            // Pipeline the inner loop over s for accumulation
            for (p = 0; p < 30; p++) {
                #pragma HLS PIPELINE II=1
                for (s = 0; s < 30; s++) {
                    #pragma HLS UNROLL factor=2
                    sum[p] += A[r][q][s] * C4[s][p];
                }
            }

            // Pipeline the loop that writes back to A
            for (p = 0; p < 30; p++) {
                #pragma HLS PIPELINE II=1
                A[r][q][p] = sum[p];
            }
        }
    }
}