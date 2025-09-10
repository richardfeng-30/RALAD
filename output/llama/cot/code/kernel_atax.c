void kernel_atax(int m, int n, float A[116][124], float x[124], float y[124], float tmp[116])
{
    #pragma HLS INTERFACE m_axi port=A offset=slave
    #pragma HLS INTERFACE m_axi port=x offset=slave
    #pragma HLS INTERFACE m_axi port=y offset=slave
    #pragma HLS INTERFACE m_axi port=tmp offset=slave
    #pragma HLS INTERFACE s_axilite port=m
    #pragma HLS INTERFACE s_axilite port=n
    #pragma HLS INTERFACE s_axilite port=A
    #pragma HLS INTERFACE s_axilite port=x
    #pragma HLS INTERFACE s_axilite port=y
    #pragma HLS INTERFACE s_axilite port=tmp
    #pragma HLS INTERFACE s_axilite port=return

    int i;
    int j;

    #pragma HLS dataflow

    // Initialize y array
    #pragma HLS loop_pipelining
    for (i = 0; i < 124; i++) {
        y[i] = 0.0;
    }

    // Calculate dot product
    #pragma HLS loop_unroll
    for (i = 0; i < 116; i++) {
        tmp[i] = 0.0;
        #pragma HLS loop_pipelining
        for (j = 0; j < 124; j++) {
            tmp[i] = tmp[i] + A[i][j] * x[j];
        }
        #pragma HLS loop_pipelining
        for (j = 0; j < 124; j++) {
            y[j] = y[j] + A[i][j] * tmp[i];
        }
    }
}