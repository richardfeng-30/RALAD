void kernel_bicg(int m, int n, float A[124][116], float s[116], float q[124], float p[116], float r[124])
{
    int i, j;

    // Initialize s array to zero with pipelined loop
    init_s:
    for (i = 0; i < 116; i++) {
        #pragma HLS PIPELINE II=1
        s[i] = 0.0f;
    }

    // Main computation
    outer_loop:
    for (i = 0; i < 124; i++) {
        #pragma HLS PIPELINE II=1
        q[i] = 0.0f;

        inner_loop:
        for (j = 0; j < 116; j++) {
            #pragma HLS PIPELINE II=1
            #pragma HLS DEPENDENCE variable=s inter false
            s[j] += r[i] * A[i][j];
            q[i] += A[i][j] * p[j];
        }
    }
}