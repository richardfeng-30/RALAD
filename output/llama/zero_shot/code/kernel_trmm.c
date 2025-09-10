void kernel_trmm(int m,int n,float alpha,float A[60][60],float B[60][80])
{
    #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
    #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
    #pragma HLS INTERFACE axis port=A
    #pragma HLS INTERFACE axis port=B
    #pragma HLS INTERFACE s_axli port=m
    #pragma HLS INTERFACE s_axli port=n
    #pragma HLS INTERFACE s_axli port=alpha
    #pragma HLS INTERFACE s_axlo port=m
    #pragma HLS INTERFACE s_axlo port=n
    #pragma HLS INTERFACE s_axlo port=alpha

    #pragma HLS dataflow

    int i;
    int j;
    int k;

    for (i = 0; i < 60; i++) {
        #pragma HLS loop_tripcount min=1 max=60
        #pragma HLS pipeline II=1
        for (j = 0; j < 80; j++) {
            #pragma HLS loop_tripcount min=1 max=80
            #pragma HLS pipeline II=1
            for (k = i + 1; k < 60; k++) {
                #pragma HLS unroll
                B[i][j] += A[k][i] * B[k][j];
            }
            B[i][j] = alpha * B[i][j];
        }
    }
}