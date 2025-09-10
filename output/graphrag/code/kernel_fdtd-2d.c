void kernel_fdtd_2d(int tmax, int nx, int ny, float ex[60][80], float ey[60][80], float hz[60][80], float _fict_[40])
{
#pragma HLS INTERFACE m_axi port=ex offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=ey offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=hz offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=_fict_ offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=tmax bundle=control
#pragma HLS INTERFACE s_axilite port=nx bundle=control
#pragma HLS INTERFACE s_axilite port=ny bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    // Partition arrays along the second dimension for parallel access
#pragma HLS array_partition variable=ex complete dim=2
#pragma HLS array_partition variable=ey complete dim=2
#pragma HLS array_partition variable=hz complete dim=2

    int t, i, j;

    // Pipeline the time loop to improve throughput
#pragma HLS pipeline II=1

    for (t = 0; t < 40; t++) {
        // Pipeline and partially unroll the loop over j for parallelism
#pragma HLS pipeline II=1
#pragma HLS unroll factor=4

        for (j = 0; j < 80; j++) {
            ey[0][j] = _fict_[t];
        }

        for (i = 1; i < 60; i++) {
#pragma HLS pipeline II=1
#pragma HLS unroll factor=4
            for (j = 0; j < 80; j++) {
                ey[i][j] = ey[i][j] - 0.5 * (hz[i][j] - hz[i - 1][j]);
            }
        }

        for (i = 0; i < 60; i++) {
#pragma HLS pipeline II=1
#pragma HLS unroll factor=4
            for (j = 1; j < 80; j++) {
                ex[i][j] = ex[i][j] - 0.5 * (hz[i][j] - hz[i][j - 1]);
            }
        }

        for (i = 0; i < 59; i++) {
#pragma HLS pipeline II=1
#pragma HLS unroll factor=4
            for (j = 0; j < 79; j++) {
                hz[i][j] = hz[i][j] - 0.7 * (ex[i][j + 1] - ex[i][j] + ey[i + 1][j] - ey[i][j]);
            }
        }
    }
}