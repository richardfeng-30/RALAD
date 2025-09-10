void kernel_gesummv(int n, float alpha, float beta, float A[90][90], float B[90][90], float tmp[90], float x[90], float y[90])
{
    #pragma HLS INTERFACE m_axi port=A offset=slave
    #pragma HLS INTERFACE m_axi port=B offset=slave
    #pragma HLS INTERFACE m_axi port=tmp offset=slave
    #pragma HLS INTERFACE m_axi port=x offset=slave
    #pragma HLS INTERFACE m_axi port=y offset=slave

    #pragma HLS INTERFACE s_axli port=n
    #pragma HLS INTERFACE s_axli port=alpha
    #pragma HLS INTERFACE s_axli port=beta
    #pragma HLS INTERFACE s_axli port=return

    #pragma HLS DATAFLOW

    #pragma HLS pipeline II=1
    #pragma HLS unroll factor=4

    for (int i = 0; i < 90; i++) {
        #pragma HLS dependence variable=tmp inter dependence
        #pragma HLS dependence variable=y inter dependence

        tmp[i] = 0.0;
        y[i] = 0.0;

        for (int j = 0; j < 90; j += 4) {
            float sum1 = A[i][j] * x[j] + A[i][j+1] * x[j+1] + A[i][j+2] * x[j+2] + A[i][j+3] * x[j+3];
            float sum2 = B[i][j] * x[j] + B[i][j+1] * x[j+1] + B[i][j+2] * x[j+2] + B[i][j+3] * x[j+3];

            tmp[i] = sum1 + tmp[i];
            y[i] = sum2 + y[i];
        }

        y[i] = alpha * tmp[i] + beta * y[i];
    }
}