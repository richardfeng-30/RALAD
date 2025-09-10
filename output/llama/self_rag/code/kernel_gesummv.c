void kernel_gesummv(int n, float alpha, float beta, float A[90][90], float B[90][90], float tmp[90], float x[90], float y[90]) {
    int i;
    int j;

    //pragma HLS INTERFACE m_axi port=A offset=slave
    //pragma HLS INTERFACE m_axi port=B offset=slave
    //pragma HLS INTERFACE m_axi port=tmp offset=slave
    //pragma HLS INTERFACE m_axi port=x offset=slave
    //pragma HLS INTERFACE m_axi port=y offset=slave

    for (i = 0; i < 90; i++) {
        #pragma HLS LOOP_TRIPCOUNT min = 90 max = 90
        tmp[i] = 0.0;
        y[i] = 0.0;

        for (j = 0; j < 90; j++) {
            #pragma HLS LOOP_TRIPCOUNT min = 90 max = 90
            tmp[i] = A[i][j] * x[j] + tmp[i];
            y[i] = B[i][j] * x[j] + y[i];
        }
        y[i] = alpha * tmp[i] + beta * y[i];
    }
}