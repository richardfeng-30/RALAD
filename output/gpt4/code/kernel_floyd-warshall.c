```c
void kernel_floyd_warshall(int n,int path[180][180])
{
#pragma HLS INLINE off
#pragma HLS ARRAY_PARTITION variable=path complete dim=2
    int i;
    int j;
    int k;
    for (k = 0; k < 180; k++) {
#pragma HLS PIPELINE II=1
        for (i = 0; i < 180; i++) {
#pragma HLS UNROLL factor=6
            for (j = 0; j < 180; j++) {
#pragma HLS UNROLL factor=6
                path[i][j] = (path[i][j] < path[i][k] + path[k][j]?path[i][j] : path[i][k] + path[k][j]);
            }
        }
    }
}
```