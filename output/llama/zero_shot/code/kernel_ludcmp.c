void kernel_ludcmp(int n,float A[120][120],float b[120],float x[120],float y[120])
{
    #pragma HLS interface m_axi port=A[0] bundle=gmem
    #pragma HLS interface m_axi port=b[0] bundle=gmem
    #pragma HLS interface m_axi port=x[0] bundle=gmem
    #pragma HLS interface m_axi port=y[0] bundle=gmem

    #pragma HLS dataflow
    int i;
    int j;
    int k;
    float w;

    for (i = 0; i < 120; i++) {
        #pragma HLS loop_tripcount max=120
        #pragma HLS pipeline II=1
        for (j = 0; j < i; j++) {
            w = A[i][j];
            #pragma HLS loop_tripcount max=i
            for (k = 0; k < j; k++) {
                w -= A[i][k] * A[k][j];
            }
            A[i][j] = w / A[j][j];
        }

        for (j = i; j < 120; j++) {
            w = A[i][j];
            #pragma HLS loop_tripcount max=120-i
            for (k = 0; k < i; k++) {
                w -= A[i][k] * A[k][j];
            }
            A[i][j] = w;
        }
    }

    for (i = 0; i < 120; i++) {
        w = b[i];
        #pragma HLS loop_tripcount max=120
        for (j = 0; j < i; j++) {
            w -= A[i][j] * y[j];
        }
        y[i] = w;
    }

    for (i = 0; i <= 119; i++) {
        int _in_i = 119 + -1 * i;
        w = y[_in_i];
        #pragma HLS loop_tripcount max=120-i
        for (j = _in_i + 1; j < 120; j++) {
            w -= A[_in_i][j] * x[j];
        }
        x[_in_i] = w / A[_in_i][_in_i];
    }
}