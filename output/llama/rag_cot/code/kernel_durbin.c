Here's the optimized code with HLS pragmas:

```c
#include <ap_fixed.h>
#include <ap_int.h>

void kernel_durbin(int n, float r[120], float y[120]) {
  #pragma HLS INTERFACE m_axi port=r offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axilite port=n bundle=control
  #pragma HLS INTERFACE s_axilite port=return bundle=control
  #pragma HLS INTERFACE s_axilite port=r[0] bundle=control
  #pragma HLS INTERFACE s_axilite port=y[0] bundle=control

  float z[120];
  float alpha;
  float beta;
  float sum;
  int i;
  int k;

  #pragma HLS DATAFLOW
  #pragma HLS pipeline II=1

  y[0] = -r[0];
  beta = 1.0;
  alpha = -r[0];

  #pragma HLS LOOP_TRIPCOUNT min=1 max=119
  #pragma HLS UNROLL
  for (k = 1; k < 120; k++) {
    beta = (((float)1) - alpha * alpha) * beta;
    sum = 0.0;
    #pragma HLS LOOP_TRIPCOUNT min=1 max=k-1
    #pragma HLS UNROLL
    for (i = 0; i < k; i++) {
      sum += r[k - i - 1] * y[i];
    }
    alpha = -(r[k] + sum) / beta;
    #pragma HLS LOOP_TRIPCOUNT min=1 max=k-1
    #pragma HLS UNROLL
    for (i = 0; i < k; i++) {
      z[i] = y[i] + alpha * y[k - i - 1];
    }
    #pragma HLS LOOP_TRIPCOUNT min=1 max=k
    #pragma HLS UNROLL
    for (i = 0; i < k; i++) {
      y[i] = z[i];
    }
    y[k] = alpha;
  }
}
```

In this code, I've added the following HLS pragmas:

1. `#pragma HLS INTERFACE` directives to specify the interfaces for the function.
2. `#pragma HLS DATAFLOW` to specify that the function is a dataflow program.
3. `#pragma HLS pipeline` to specify the pipeline depth.
4. `#pragma HLS LOOP_TRIPCOUNT` directives to specify the loop trip count.
5. `#pragma HLS UNROLL` directives to specify the loop unrolling factor.

The HLS pragmas are used to optimize the code for performance and resource usage. The `#pragma HLS INTERFACE` directives specify the interfaces for the function, which allows the compiler to generate optimized code for the specific interfaces. The `#pragma HLS DATAFLOW` directive specifies that the function is a dataflow program, which allows the compiler to generate optimized code for dataflow programs.

The `#pragma HLS pipeline` directive specifies the pipeline depth, which allows the compiler to generate optimized code for pipelined programs. The `#pragma HLS LOOP_TRIPCOUNT` directives specify the loop trip count, which allows the compiler to generate optimized code for loops with known trip counts. The `#pragma HLS UNROLL` directives specify the loop unrolling factor, which allows the compiler to generate optimized code for loops with known trip counts.

By using these HLS pragmas, the compiler can generate optimized code that is more efficient and uses fewer resources than the original code.