void kernel_floyd_warshall(int n, int path[180][180]) {
  #pragma HLS interface m_axi port = path[0] bundle = s_axi
  #pragma HLS dataflow
  
  int i;
  int j;
  int k;
  
  {
    #pragma HLS loop_tripcount min = 1 max = 180
    for (k = 0; k < 180; k++) {
      
      #pragma HLS loop_tripcount min = 1 max = 180
      for (i = 0; i < 180; i++) {
        
        #pragma HLS loop_tripcount min = 1 max = 180
        for (j = 0; j < 180; j++) {
          path[i][j] = (path[i][j] < path[i][k] + path[k][j]?path[i][j] : path[i][k] + path[k][j]);
        }
      }
    }
  }
}