void kernel_3mm(int ni,int nj,int nk,int nl,int nm,float E[40][50],float A[40][60],float B[60][50],float F[50][70],float C[50][80],float D[80][70],float G[40][70])
{
    #pragma HLS INTERFACE m_axis port=E
    #pragma HLS INTERFACE m_axis port=A
    #pragma HLS INTERFACE m_axis port=B
    #pragma HLS INTERFACE m_axis port=F
    #pragma HLS INTERFACE m_axis port=C
    #pragma HLS INTERFACE m_axis port=D
    #pragma HLS INTERFACE m_axis port=G

    int i;
    int j;
    int k;

    #pragma HLS DATAFLOW

    for (i = 0; i < 40; i++) {
        #pragma HLS LOOP_TRIPCOUNT min=40 max=40
        #pragma HLS UNROLL
        for (j = 0; j < 50; j++) {
            #pragma HLS LOOP_TRIPCOUNT min=50 max=50
            E[i][j] = 0.0;
            for (k = 0; k < 60; ++k) {
                #pragma HLS LOOP_TRIPCOUNT min=60 max=60
                E[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    for (i = 0; i < 50; i++) {
        #pragma HLS LOOP_TRIPCOUNT min=50 max=50
        #pragma HLS UNROLL
        for (j = 0; j < 70; j++) {
            #pragma HLS LOOP_TRIPCOUNT min=70 max=70
            F[i][j] = 0.0;
            for (k = 0; k < 80; ++k) {
                #pragma HLS LOOP_TRIPCOUNT min=80 max=80
                F[i][j] += C[i][k] * D[k][j];
            }
        }
    }

    for (i = 0; i < 40; i++) {
        #pragma HLS LOOP_TRIPCOUNT min=40 max=40
        #pragma HLS UNROLL
        for (j = 0; j < 70; j++) {
            #pragma HLS LOOP_TRIPCOUNT min=70 max=70
            G[i][j] = 0.0;
            for (k = 0; k < 50; ++k) {
                #pragma HLS LOOP_TRIPCOUNT min=50 max=50
                G[i][j] += E[i][k] * F[k][j];
            }
        }
    }
}