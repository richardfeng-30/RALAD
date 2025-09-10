void kernel_atax(int m, int n, float A[116][124], float x[124], float y[124], float tmp[116])
{
    #pragma HLS INTERFACE m_axi port=A offset=slave
    #pragma HLS INTERFACE m_axi port=x offset=slave
    #pragma HLS INTERFACE m_axi port=y offset=slave
    #pragma HLS INTERFACE m_axi port=tmp offset=slave
    #pragma HLS INTERFACE s_axli port=m
    #pragma HLS INTERFACE s_axli port=n
    #pragma HLS INTERFACE s_axli port=return

    int i;
    int j;

    #pragma HLS DATAFLOW

    #pragma HLS loop_tripcount min=1 max=116
    for (i = 0; i < 124; i++) {
        y[i] = 0.0;
    }

    #pragma HLS loop_tripcount min=1 max=116
    for (i = 0; i < 116; i++) {
        tmp[i] = 0.0;

        #pragma HLS loop_tripcount min=1 max=124
        for (j = 0; j < 124; j++) {
            tmp[i] = tmp[i] + A[i][j] * x[j];
        }

        #pragma HLS loop_tripcount min=1 max=124
        for (j = 0; j < 124; j++) {
            y[j] = y[j] + A[i][j] * tmp[i];
        }
    }
}