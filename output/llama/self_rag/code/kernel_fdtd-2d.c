void kernel_fdtd_2d(int tmax, int nx, int ny, float ex[60][80], float ey[60][80], float hz[60][80], float _fict_[40]) {
  #pragma HLS INTERFACE m_axi port=ex offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=ey offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=hz offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=_fict_ offset=slave bundle=gmem

  int t;
  int i;
  int j;

  #pragma HLS DATAFLOW

  // Loop unrolling for better pipeline efficiency
  #pragma HLS LOOP_TRIPCOUNT min=40 max=40
  #pragma HLS PIPELINE II=1
  for (t = 0; t < 40; t++) {
    #pragma HLS LOOP_TRIPCOUNT min=80 max=80
    for (j = 0; j < 80; j++) {
      ey[0][j] = _fict_[t];
    }

    #pragma HLS LOOP_TRIPCOUNT min=59 max=59
    for (i = 1; i < 60; i++) {
      #pragma HLS LOOP_TRIPCOUNT min=79 max=79
      for (j = 0; j < 80; j++) {
        ey[i][j] = ey[i][j] - 0.5 * (hz[i][j] - hz[i - 1][j]);
      }
    }

    #pragma HLS LOOP_TRIPCOUNT min=59 max=59
    for (i = 0; i < 60; i++) {
      #pragma HLS LOOP_TRIPCOUNT min=79 max=79
      for (j = 1; j < 80; j++) {
        ex[i][j] = ex[i][j] - 0.5 * (hz[i][j] - hz[i][j - 1]);
      }
    }

    #pragma HLS LOOP_TRIPCOUNT min=59 max=59
    for (i = 0; i < 59; i++) {
      #pragma HLS LOOP_TRIPCOUNT min=79 max=79
      for (j = 0; j < 79; j++) {
        hz[i][j] = hz[i][j] - 0.7 * (ex[i][j + 1] - ex[i][j] + ey[i + 1][j] - ey[i][j]);
      }
    }
  }
}