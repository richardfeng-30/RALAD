```c
void kernel_3mm(int ni,int nj,int nk,int nl,int nm,float E[40][50],float A[40][60],float B[60][50],float F[50][70],float C[50][80],float D[80][70],float G[40][70])
{
#pragma HLS INTERFACE m_axi port=E offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem2
#pragma HLS INTERFACE m_axi port=F offset=slave bundle=gmem3
#pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem4
#pragma HLS INTERFACE m_axi port=D offset=slave bundle=gmem5
#pragma HLS INTERFACE m_axi port=G offset=slave bundle=gmem6
#pragma HLS INTERFACE s_axilite port=ni bundle=control
#pragma HLS INTERFACE s_axilite port=nj bundle=control
#pragma HLS INTERFACE s_axilite port=nk bundle=control
#pragma HLS INTERFACE s_axilite port=nl bundle=control
#pragma HLS INTERFACE s_axilite port=nm bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS ARRAY_PARTITION variable=A complete dim=2
#pragma HLS ARRAY_PARTITION variable=B complete dim=1
#pragma HLS ARRAY_PARTITION variable=C complete dim=2
#pragma HLS ARRAY_PARTITION variable=D complete dim=1
#pragma HLS ARRAY_PARTITION variable=E complete dim=2
#pragma HLS ARRAY_PARTITION variable=F complete dim=1

  int i;
  int j;
  int k;
  {

    // E = A * B
    for (i = 0; i < 40; i++) {
#pragma HLS PIPELINE II=1
      for (j = 0; j < 50; j++) {
#pragma HLS UNROLL factor=2
        E[i][j] = 0.0;
        for (k = 0; k < 60; ++k) {
#pragma HLS UNROLL factor=2
          E[i][j] += A[i][k] * B[k][j];
        }
      }
    }

    // F = C * D
    for (i = 0; i < 50; i++) {
#pragma HLS PIPELINE II=1
      for (j = 0; j < 70; j++) {
#pragma HLS UNROLL factor=2
        F[i][j] = 0.0;
        for (k = 0; k < 80; ++k) {
#pragma HLS UNROLL factor=2
          F[i][j] += C[i][k] * D[k][j];
        }
      }
    }

    // G = E * F
    for (i = 0; i < 40; i++) {
#pragma HLS PIPELINE II=1
      for (j = 0; j < 70; j++) {
#pragma HLS UNROLL factor=2
        G[i][j] = 0.0;
        for (k = 0; k < 50; ++k) {
#pragma HLS UNROLL factor=2
          G[i][j] += E[i][k] * F[k][j];
        }
      }
    }
  }
}
```