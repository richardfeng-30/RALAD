void kernel_mvt(int n, float x1[120], float x2[120], float y_1[120], float y_2[120], float A[120][120]) {
    #pragma HLS INTERFACE m_axis port=x1 offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axis port=x2 offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axis port=y_1 offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axis port=y_2 offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axis port=A offset=slave bundle=gmem

    int i;
    int j;

    #pragma HLS PIPELINE II=1
    for (i = 0; i < 120; i += 4) {
        for (j = 0; j < 120; j += 4) {
            x1[i] = x1[i] + A[i][j] * y_1[j];
            x1[i+1] = x1[i+1] + A[i+1][j] * y_1[j+1];
            x1[i+2] = x1[i+2] + A[i+2][j] * y_1[j+2];
            x1[i+3] = x1[i+3] + A[i+3][j] * y_1[j+3];
        }
    }

    #pragma HLS PIPELINE II=1
    for (i = 0; i < 120; i += 4) {
        for (j = 0; j < 120; j += 4) {
            x2[i] = x2[i] + A[j][i] * y_2[j];
            x2[i+1] = x2[i+1] + A[j+1][i] * y_2[j+1];
            x2[i+2] = x2[i+2] + A[j+2][i] * y_2[j+2];
            x2[i+3] = x2[i+3] + A[j+3][i] * y_2[j+3];
        }
    }
}