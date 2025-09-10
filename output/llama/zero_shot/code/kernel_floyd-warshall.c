void kernel_floyd_warshall(int n, int path[180][180])
{
  #pragma HLS INTERFACE m_axi port=path offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axilite port=n
  #pragma HLS INTERFACE s_axilite port=return

  int i;
  int j;
  int k;

  #pragma HLS DATAFLOW
  #pragma HLS depend variable path

  for (k = 0; k < 180; k++) {
    #pragma HLS PIPELINE II=1
    for (i = 0; i < 180; i++) {
      for (j = 0; j < 180; j++) {
        path[i][j] = (path[i][j] < path[i][k] + path[k][j]?path[i][j] : path[i][k] + path[k][j]);
      }
    }
  }
}