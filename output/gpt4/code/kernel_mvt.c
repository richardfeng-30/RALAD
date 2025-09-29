void kernel_mvt(int n,float x1[120],float x2[120],float y_1[120],float y_2[120],float A[120][120])
{
#pragma HLS INTERFACE m_axi port=x1 offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=x2 offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=y_1 offset=slave bundle=gmem2
#pragma HLS INTERFACE m_axi port=y_2 offset=slave bundle=gmem3
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem4
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS ARRAY_PARTITION variable=A cyclic factor=10 dim=2
#pragma HLS ARRAY_PARTITION variable=A cyclic factor=10 dim=1
#pragma HLS ARRAY_PARTITION variable=y_1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=y_2 complete dim=1

    int i;
    int j;

    for (i = 0; i < 120; i++) {
#pragma HLS PIPELINE II=1
        float sum = x1[i];
        for (j = 0; j < 120; j++) {
#pragma HLS UNROLL factor=10
            sum += A[i][j] * y_1[j];
        }
        x1[i] = sum;
    }

    for (i = 0; i < 120; i++) {
#pragma HLS PIPELINE II=1
        float sum = x2[i];
        for (j = 0; j < 120; j++) {
#pragma HLS UNROLL factor=10
            sum += A[j][i] * y_2[j];
        }
        x2[i] = sum;
    }
}