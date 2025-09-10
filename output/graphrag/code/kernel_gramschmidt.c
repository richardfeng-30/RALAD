#include <math.h>

void kernel_gramschmidt(int m, int n, float A[60][80], float R[80][80], float Q[60][80])
{
    int i, j, k;
    float nrm;

    // Partition arrays along the first dimension for parallel access
    #pragma HLS ARRAY_PARTITION variable=A complete dim=1
    #pragma HLS ARRAY_PARTITION variable=Q complete dim=1

    for (k = 0; k < 80; k++) {
        nrm = 0.0;

        // Pipeline the loop to accumulate norm
        #pragma HLS PIPELINE II=1
        for (i = 0; i < 60; i++) {
            nrm += A[i][k] * A[i][k];
        }
        R[k][k] = sqrt(nrm);

        // Pipeline the loop to compute Q
        #pragma HLS PIPELINE II=1
        for (i = 0; i < 60; i++) {
            Q[i][k] = A[i][k] / R[k][k];
        }

        for (j = k + 1; j < 80; j++) {
            R[k][j] = 0.0;

            // Pipeline the loop to compute R[k][j]
            #pragma HLS PIPELINE II=1
            for (i = 0; i < 60; i++) {
                R[k][j] += Q[i][k] * A[i][j];
            }

            // Pipeline the loop to update A
            #pragma HLS PIPELINE II=1
            for (i = 0; i < 60; i++) {
                A[i][j] = A[i][j] - Q[i][k] * R[k][j];
            }
        }
    }
}