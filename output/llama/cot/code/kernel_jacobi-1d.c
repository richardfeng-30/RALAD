void kernel_jacobi_1d(int tsteps,int n,float A[120],float B[120])
{
    #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
    #pragma HLS INTERFACE s_axli port=tsteps offset=slave bundle=control
    #pragma HLS INTERFACE s_axli port=n offset=slave bundle=control
    #pragma HLS INTERFACE s_axlo port=A offset=slave bundle=control
    #pragma HLS INTERFACE s_axlo port=B offset=slave bundle=control

    #pragma HLS DATAFLOW

    #pragma HLS pipeline II=4

    int t;
    int i;
    for (t = 0; t < 40; t += 4) {
        #pragma HLS loop_unroll
        for (i = 1; i < 120 - 1; i += 4) {
            B[i] = 0.33333 * (A[i - 1] + A[i] + A[i + 1]);
            B[i + 1] = 0.33333 * (A[i - 1] + A[i] + A[i + 1]);
            B[i + 2] = 0.33333 * (A[i - 1] + A[i] + A[i + 1]);
            B[i + 3] = 0.33333 * (A[i - 1] + A[i] + A[i + 1]);
        }
        #pragma HLS loop_unroll
        for (i = 1; i < 120 - 1; i += 4) {
            A[i] = 0.33333 * (B[i - 1] + B[i] + B[i + 1]);
            A[i + 1] = 0.33333 * (B[i - 1] + B[i] + B[i + 1]);
            A[i + 2] = 0.33333 * (B[i - 1] + B[i] + B[i + 1]);
            A[i + 3] = 0.33333 * (B[i - 1] + B[i] + B[i + 1]);
        }
    }
}