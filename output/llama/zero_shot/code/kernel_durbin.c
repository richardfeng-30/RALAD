void kernel_durbin(int n, float r[120], float y[120])
{
    #pragma HLS INTERFACE m_axi port=r offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem
    #pragma HLS INTERFACE s_axli offset=slave bundle=gmem
    #pragma HLS INTERFACE s_axlo offset=slave bundle=gmem

    #pragma HLS DATAFLOW

    float z[120];
    float alpha;
    float beta;
    float sum;
    int i;
    int k;

    #pragma HLS DATA_PACK variable=r
    #pragma HLS DATA_PACK variable=y
    #pragma HLS DATA_PACK variable=z

    {
        y[0] = -r[0];
        beta = 1.0;
        alpha = -r[0];

        #pragma HLS LOOP_TRIPCOUNT min=119 max=119
        for (k = 1; k < 120; k++) {
            #pragma HLS LOOP_TRIPCOUNT min=118 max=118
            beta = (((float )1) - alpha * alpha) * beta;
            sum = 0.0;
            #pragma HLS LOOP_TRIPCOUNT min=118 max=118
            for (i = 0; i < k; i++) {
                sum += r[k - i - 1] * y[i];
            }
            alpha = -(r[k] + sum) / beta;
            #pragma HLS LOOP_TRIPCOUNT min=118 max=118
            for (i = 0; i < k; i++) {
                z[i] = y[i] + alpha * y[k - i - 1];
            }
            #pragma HLS LOOP_TRIPCOUNT min=118 max=118
            for (i = 0; i < k; i++) {
                y[i] = z[i];
            }
            y[k] = alpha;
        }
    }
}