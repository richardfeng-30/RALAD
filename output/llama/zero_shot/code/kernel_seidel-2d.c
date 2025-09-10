void kernel_seidel_2d(int tsteps, int n, float A[120][120])
{
    #pragma HLS INTERFACE m_axi port=A[0] offset=slave bundle=gmem
    #pragma HLS INTERFACE axis port=A[0]
    #pragma HLS INTERFACE s_axilite port=tsteps
    #pragma HLS INTERFACE s_axilite port=n
    #pragma HLS INTERFACE s_axilite port=A

    int t;
    int i;
    int j;

    #pragma HLS DATAFLOW

    #pragma HLS LOOP_FLATTEN min_loop_count=1
    #pragma HLS LOOP_TRIPCOUNT min=41 max=41 avg=41
    for (t = 0; t <= 40 - 1; t++) {
        #pragma HLS LOOP_FLATTEN min_loop_count=1
        #pragma HLS LOOP_TRIPCOUNT min=119 max=119 avg=119
        for (i = 1; i <= 120 - 2; i++) {
            #pragma HLS LOOP_FLATTEN min_loop_count=1
            #pragma HLS LOOP_TRIPCOUNT min=119 max=119 avg=119
            for (j = 1; j <= 120 - 2; j++) {
                A[i][j] = (A[i - 1][j - 1] + A[i - 1][j] + A[i - 1][j + 1] + A[i][j - 1] + A[i][j] + A[i][j + 1] + A[i + 1][j - 1] + A[i + 1][j] + A[i + 1][j + 1]) / 9.0;
            }
        }
    }
}