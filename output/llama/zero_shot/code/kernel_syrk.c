void kernel_syrk(int n, int m, float alpha, float beta, float C[80][80], float A[80][60])
{
    #pragma HLS interface m_axi port=C[0] dim=2
    #pragma HLS interface m_axi port=A[0] dim=2
    
    int i;
    int j;
    int k;
    
    //BLAS PARAMS
    //TRANS = 'N'
    //UPLO  = 'L'
    // =>  Form  C := alpha*A*A**T + beta*C.
    //A is NxM
    //C is NxN
    
    #pragma HLS dataflow
    
    for (i = 0; i < 80; i++) {
        #pragma HLS loop_tripcount min=1 max=80
        for (j = 0; j <= i; j++) {
            #pragma HLS loop_tripcount min=1 max=80
            C[i][j] *= beta;
        }
        
        for (k = 0; k < 60; k++) {
            #pragma HLS loop_tripcount min=1 max=60
            for (j = 0; j <= i; j++) {
                #pragma HLS loop_tripcount min=1 max=80
                C[i][j] += alpha * A[i][k] * A[j][k];
            }
        }
    }
}