#include <math.h>

void kernel_cholesky(int n, float A[120][120])
{
    #pragma HLS INTERFACE m_axi port=A[0] depth=120
    #pragma HLS INTERFACE s_axilite port=n
    #pragma HLS INTERFACE s_axilite port=return

    int i;
    int j;
    int k;

    #pragma HLS DATAFLOW

    for (i = 0; i < 120; i++) {
        #pragma HLS LOOP_FLATTEN
        for (j = 0; j < i; j++) {
            #pragma HLS LOOP_FLATTEN
            for (k = 0; k < j; k++) {
                A[i][j] -= A[i][k] * A[j][k];
            }
            A[i][j] /= A[j][j];
        }
        #pragma HLS LOOP_FLATTEN
        for (k = 0; k < i; k++) {
            A[i][i] -= A[i][k] * A[i][k];
        }
        A[i][i] = sqrt(A[i][i]);
    }
}