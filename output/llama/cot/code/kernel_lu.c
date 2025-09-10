void kernel_lu(int n, float A[120][120])
{
    #pragma HLS INTERFACE m_axi port=A[0] offset=slave bundle=gmem
    #pragma HLS INTERFACE s_axilite port=n
    #pragma HLS INTERFACE s_axilite port=return

    int i;
    int j;
    int k;

    #pragma HLS DATAFLOW

    for (i = 0; i < 120; i=i+4) {
        #pragma HLS LOOP_TRIPCOUNT min=30 max=30
        #pragma HLS LOOP_FLATTEN min=4
        for (j = 0; j < i; j=j+4) {
            #pragma HLS LOOP_TRIPCOUNT min=7 max=7
            #pragma HLS LOOP_FLATTEN min=4
            for (k = 0; k < j; k=k+4) {
                #pragma HLS LOOP_TRIPCOUNT min=7 max=7
                #pragma HLS LOOP_FLATTEN min=4
                A[i][j] -= A[i][k] * A[k][j];
            }
            A[i][j] /= A[j][j];
        }
        for (j = i; j < 120; j=j+4) {
            #pragma HLS LOOP_TRIPCOUNT min=91 max=91
            #pragma HLS LOOP_FLATTEN min=4
            for (k = 0; k < i; k=k+4) {
                #pragma HLS LOOP_TRIPCOUNT min=91 max=91
                #pragma HLS LOOP_FLATTEN min=4
                A[i][j] -= A[i][k] * A[k][j];
            }
        }
    }
}