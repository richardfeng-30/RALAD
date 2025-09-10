#include <math.h>

void kernel_correlation(int m, int n, float float_n, float data[100][80], float corr[80][80], float mean[80], float stddev[80])
{
    float eps = 0.1;
    int i, j, k;

    // Partition arrays along the second dimension for parallel access
#pragma HLS ARRAY_PARTITION variable=data complete dim=2
#pragma HLS ARRAY_PARTITION variable=mean complete dim=1
#pragma HLS ARRAY_PARTITION variable=stddev complete dim=1
#pragma HLS ARRAY_PARTITION variable=corr complete dim=2

    // Compute mean
    for (j = 0; j < 80; j++) {
#pragma HLS PIPELINE II=1
        mean[j] = 0.0;
        for (i = 0; i < 100; i++) {
#pragma HLS PIPELINE II=1
            mean[j] += data[i][j];
        }
        mean[j] /= float_n;
    }

    // Compute stddev
    for (j = 0; j < 80; j++) {
#pragma HLS PIPELINE II=1
        stddev[j] = 0.0;
        for (i = 0; i < 100; i++) {
#pragma HLS PIPELINE II=1
            float diff = data[i][j] - mean[j];
            stddev[j] += diff * diff;
        }
        stddev[j] /= float_n;
        stddev[j] = sqrt(stddev[j]);
        stddev[j] = (stddev[j] <= eps ? 1.0 : stddev[j]);
    }

    // Normalize data
    for (i = 0; i < 100; i++) {
#pragma HLS PIPELINE II=1
        for (j = 0; j < 80; j++) {
#pragma HLS UNROLL factor=4
            data[i][j] -= mean[j];
            data[i][j] /= sqrt(float_n) * stddev[j];
        }
    }

    // Compute correlation matrix
    for (i = 0; i < 79; i++) {
#pragma HLS PIPELINE II=1
        corr[i][i] = 1.0;
        for (j = i + 1; j < 80; j++) {
#pragma HLS PIPELINE II=1
            corr[i][j] = 0.0;
            for (k = 0; k < 100; k++) {
#pragma HLS PIPELINE II=1
                corr[i][j] += data[k][i] * data[k][j];
            }
            corr[j][i] = corr[i][j];
        }
    }
    corr[79][79] = 1.0;
}