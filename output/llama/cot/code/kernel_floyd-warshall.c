void kernel_floyd_warshall(int n, int path[180][180])
{
    #pragma HLS INTERFACE m_axi port=path[0] depth=180
    #pragma HLS INTERFACE axis port=path[0]
    #pragma HLS INTERFACE s_axilite port=n
    #pragma HLS INTERFACE s_axilite port=return

    int i;
    int j;
    int k;
    #pragma HLS loop_tripcount min=1 max=180
    #pragma HLS loop_tripcount avg=90
    #pragma HLS loop_pipelined off

    for (k = 0; k < 180; k += 4) {
        #pragma HLS loop_tripcount min=1 max=45
        #pragma HLS loop_tripcount avg=22.5
        #pragma HLS loop_pipelined off
        for (i = 0; i < 180; i += 4) {
            #pragma HLS loop_tripcount min=1 max=45
            #pragma HLS loop_tripcount avg=22.5
            #pragma HLS loop_pipelined off
            for (j = 0; j < 180; j += 4) {
                #pragma HLS loop_tripcount min=1 max=45
                #pragma HLS loop_tripcount avg=22.5
                #pragma HLS loop_pipelined off
                path[i][j] = (path[i][j] < path[i][k] + path[k][j]?path[i][j] : path[i][k] + path[k][j]);
            }
        }
    }
}