```c
void kernel_fdtd_2d(int tmax,int nx,int ny,float ex[60][80],float ey[60][80],float hz[60][80],float _fict_[40])
{
#pragma HLS INTERFACE m_axi port=ex offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=ey offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=hz offset=slave bundle=gmem2
#pragma HLS INTERFACE m_axi port=_fict_ offset=slave bundle=gmem3
#pragma HLS INTERFACE s_axilite port=ex bundle=control
#pragma HLS INTERFACE s_axilite port=ey bundle=control
#pragma HLS INTERFACE s_axilite port=hz bundle=control
#pragma HLS INTERFACE s_axilite port=_fict_ bundle=control
#pragma HLS INTERFACE s_axilite port=tmax bundle=control
#pragma HLS INTERFACE s_axilite port=nx bundle=control
#pragma HLS INTERFACE s_axilite port=ny bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  int t;
  int i;
  int j;
{
    for (t = 0; t < 40; t++) {
#pragma HLS LOOP_TRIPCOUNT min=40 max=40
      for (j = 0; j < 80; j++) {
#pragma HLS PIPELINE II=1
        ey[0][j] = _fict_[t];
      }

      for (i = 1; i < 60; i++) {
#pragma HLS LOOP_TRIPCOUNT min=59 max=59
        for (j = 0; j < 80; j++) {
#pragma HLS PIPELINE II=1
          ey[i][j] = ey[i][j] - 0.5 * (hz[i][j] - hz[i - 1][j]);
        }
      }

      for (i = 0; i < 60; i++) {
#pragma HLS LOOP_TRIPCOUNT min=60 max=60
        for (j = 1; j < 80; j++) {
#pragma HLS PIPELINE II=1
          ex[i][j] = ex[i][j] - 0.5 * (hz[i][j] - hz[i][j - 1]);
        }
      }

      for (i = 0; i < 59; i++) {
#pragma HLS LOOP_TRIPCOUNT min=59 max=59
        for (j = 0; j < 79; j++) {
#pragma HLS PIPELINE II=1
          hz[i][j] = hz[i][j] - 0.7 * (ex[i][j + 1] - ex[i][j] + ey[i + 1][j] - ey[i][j]);
        }
      }
    }
  }
}
```