void kernel_covariance(int m, int n, float float_n, float data[100][80], float cov[80][80], float mean[80])
{
    int i, j, k;

    // Partition the data array along the second dimension for parallel access
    #pragma HLS ARRAY_PARTITION variable=data complete dim=2
    // Partition the cov array along both dimensions for parallel access
    #pragma HLS ARRAY_PARTITION variable=cov complete dim=2
    #pragma HLS ARRAY_PARTITION variable=cov complete dim=1
    // Partition mean array for parallel access
    #pragma HLS ARRAY_PARTITION variable=mean complete dim=1

    // Calculate mean of each column
    for (j = 0; j < 80; j++) {
        mean[j] = 0.0;
        #pragma HLS PIPELINE II=1
        for (i = 0; i < 100; i++) {
            mean[j] += data[i][j];
        }
        mean[j] /= float_n;
    }

    // Subtract mean from data
    for (i = 0; i < 100; i++) {
        #pragma HLS PIPELINE II=1
        for (j = 0; j < 80; j++) {
            data[i][j] -= mean[j];
        }
    }

    // Compute covariance matrix
    for (i = 0; i < 80; i++) {
        for (j = i; j < 80; j++) {
            cov[i][j] = 0.0;
            #pragma HLS PIPELINE II=1
            for (k = 0; k < 100; k++) {
                cov[i][j] += data[k][i] * data[k][j];
            }
            cov[i][j] /= (float_n - 1.0);
            cov[j][i] = cov[i][j];
        }
    }
}