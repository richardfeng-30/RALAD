void kernel_gemm(int ni, int nj, int nk, float alpha, float beta, float C[60][70], float A[60][80], float B[80][70])
{
#pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem2
#pragma HLS INTERFACE s_axilite port=ni bundle=control
#pragma HLS INTERFACE s_axilite port=nj bundle=control
#pragma HLS INTERFACE s_axilite port=nk bundle=control
#pragma HLS INTERFACE s_axilite port=alpha bundle=control
#pragma HLS INTERFACE s_axilite port=beta bundle=control
#pragma HLS INTERFACE s_axilite port=C bundle=control
#pragma HLS INTERFACE s_axilite port=A bundle=control
#pragma HLS INTERFACE s_axilite port=B bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    // Partition arrays to enable parallel access
#pragma HLS ARRAY_PARTITION variable=A cyclic factor=8 dim=2
#pragma HLS ARRAY_PARTITION variable=B cyclic factor=8 dim=1
#pragma HLS ARRAY_PARTITION variable=C cyclic factor=8 dim=2

    // Pipeline the outer loop to improve throughput
    for (int i = 0; i < 60; i++) {
#pragma HLS PIPELINE II=1

        // Scale C by beta
        for (int j = 0; j < 70; j++) {
#pragma HLS UNROLL factor=4
            C[i][j] *= beta;
        }

        // Compute alpha * A * B and accumulate into C
        for (int k = 0; k < 80; k++) {
#pragma HLS PIPELINE II=1
            for (int j = 0; j < 70; j++) {
#pragma HLS UNROLL factor=4
                C[i][j] += alpha * A[i][k] * B[k][j];
            }
        }
    }
}