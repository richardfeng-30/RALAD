void kernel_gemm(int ni,int nj,int nk,float alpha,float beta,float C[60][70],float A[60][80],float B[80][70])
{
    #pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem

    int i;
    int j;
    int k;
    float temp[70];

    #pragma HLS DATAFLOW

    // Initialize C with beta
    for (i = 0; i < 60; i++) {
        #pragma HLS LOOP_TRIPCOUNT min=1 max=60
        #pragma HLS PIPELINE II=1
        for (j = 0; j < 70; j++) {
            temp[j] = C[i][j] * beta;
        }
        #pragma HLS ARRAY_PARTITION variable=temp cyclic factor=4
        #pragma HLS DATAFLOW
        for (j = 0; j < 70; j++) {
            C[i][j] = temp[j];
        }
    }

    // Perform matrix multiplication
    for (i = 0; i < 60; i++) {
        #pragma HLS LOOP_TRIPCOUNT min=1 max=60
        for (k = 0; k < 80; k++) {
            #pragma HLS LOOP_TRIPCOUNT min=1 max=80
            for (j = 0; j < 70; j++) {
                #pragma HLS PIPELINE II=1
                temp[j] = alpha * A[i][k] * B[k][j];
            }
            #pragma HLS ARRAY_PARTITION variable=temp cyclic factor=4
            #pragma HLS DATAFLOW
            for (j = 0; j < 70; j++) {
                C[i][j] += temp[j];
            }
        }
    }
}