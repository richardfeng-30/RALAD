Here's the optimized code with HLS pragmas:

```c
void kernel_adi(int tsteps, int n, float u[60][60], float v[60][60], float p[60][60], float q[60][60]) {
  #pragma HLS INTERFACE m_axi port=u offset=slave
  #pragma HLS INTERFACE m_axi port=v offset=slave
  #pragma HLS INTERFACE m_axi port=p offset=slave
  #pragma HLS INTERFACE m_axi port=q offset=slave
  #pragma HLS INTERFACE s_axilite port=tsteps
  #pragma HLS INTERFACE s_axilite port=n
  #pragma HLS INTERFACE s_axilite port=u
  #pragma HLS INTERFACE s_axilite port=v
  #pragma HLS INTERFACE s_axilite port=p
  #pragma HLS INTERFACE s_axilite port=q
  #pragma HLS INTERFACE s_axilite port=return

  float DX;
  float DY;
  float DT;
  float B1;
  float B2;
  float mul1;
  float mul2;
  float a;
  float b;
  float c;
  float d;
  float e;
  float f;
  int t;
  int i;
  int j;

  {
    DX = 1.0 / ((float)60);
    DY = 1.0 / ((float)60);
    DT = 1.0 / ((float)40);
    B1 = 2.0;
    B2 = 1.0;
    mul1 = B1 * DT / (DX * DX);
    mul2 = B2 * DT / (DY * DY);
    a = -mul1 / 2.0;
    b = 1.0 + mul1;
    c = a;
    d = -mul2 / 2.0;
    e = 1.0 + mul2;
    f = d;

    #pragma HLS dataflow
    for (t = 1; t <= 40; t++) {
      //Column Sweep
      #pragma HLS pipeline II=1
      for (i = 1; i < 60 - 1; i++) {
        v[0][i] = 1.0;
        p[i][0] = 0.0;
        q[i][0] = v[0][i];

        #pragma HLS loop_tripcount min=59 max=59
        #pragma HLS pipeline II=1
        for (j = 1; j < 60 - 1; j++) {
          p[i][j] = -c / (a * p[i][j - 1] + b);
          q[i][j] = (-d * u[j][i - 1] + (1.0 + 2.0 * d) * u[j][i] - f * u[j][i + 1] - a * q[i][j - 1]) / (a * p[i][j - 1] + b);
        }
        v[60 - 1][i] = 1.0;

        #pragma HLS loop_tripcount min=58 max=58
        #pragma HLS pipeline II=1
        for (j = 0; j <= 57; j++) {
          int _in_j_0 = 58 + -1 * j;
          v[_in_j_0][i] = p[i][_in_j_0] * v[_in_j_0 + 1][i] + q[i][_in_j_0];
        }
        j = 1 + -1;
      }
      //Row Sweep
      #pragma HLS pipeline II=1
      for (i = 1; i < 60 - 1; i++) {
        u[i][0] = 1.0;
        p[i][0] = 0.0;
        q[i][0] = u[i][0];

        #pragma HLS loop_tripcount min=59 max=59
        #pragma HLS pipeline II=1
        for (j = 1; j < 60 - 1; j++) {
          p[i][j] = -f / (d * p[i][j - 1] + e);
          q[i][j] = (-a * v[i - 1][j] + (1.0 + 2.0 * a) * v[i][j] - c * v[i + 1][j] - d * q[i][j - 1]) / (d * p[i][j - 1] + e);
        }
        u[i][60 - 1] = 1.0;

        #pragma HLS loop_tripcount min=58 max=58
        #pragma HLS pipeline II=1
        for (j = 0; j <= 57; j++) {
          int _in_j = 58 + -1 * j;
          u[i][_in_j] = p[i][_in_j] * u[i][_in_j + 1] + q[i][_in_j];
        }
        j = 1 + -1;
      }
    }
  }
}
```

Explanation:
1. The first step is to add pragmas for the interfaces. This is because the kernel_adi function is assumed to be a part of a larger system that communicates with other components.
2. The next step is to add pragmas for the loops. This is because the loops in the kernel_adi function are the most critical part of the function and should be optimized for performance.
3. The pragmas for the loops include the `#pragma HLS pipeline` directive, which is used to pipeline the loops. This can significantly improve the performance of the function by reducing the latency of the loops.
4. The `#pragma HLS loop_tripcount` directive is used to specify the minimum and maximum number of iterations for each loop. This can help the compiler to generate more efficient code.
5. Finally, the pragmas for the memory access include the `#pragma HLS dataflow` directive, which is used to specify the memory access pattern. This can help the compiler to generate more efficient code for the memory access.

By adding these pragmas, the kernel_adi function can be optimized for performance and can be used in a larger system that communicates with other components.