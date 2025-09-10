The given code is a Cholesky decomposition algorithm, which is a part of the LU decomposition algorithm. This algorithm is used to decompose a symmetric positive-definite matrix into a lower triangular matrix and its transpose.

To optimize this code for HLS, we can use the following pragmas:

1.  `#pragma HLS pipeline II=1` to enable pipelining, which allows the compiler to break the loop into multiple stages, each stage processing one iteration of the loop. This can improve the throughput of the code.

2.  `#pragma HLS loop_flatten` to flatten the nested loops into a single loop, which can improve the performance of the code.

3.  `#pragma HLS array_partition` to partition the arrays into smaller chunks, which can improve the memory access patterns and reduce the memory bandwidth.

4.  `#pragma HLS dataflow` to enable dataflow execution, which allows the compiler to execute the code in a dataflow manner, where each statement is executed as soon as its inputs are available.

Here is the optimized code with HLS pragmas:

```c
#include <math.h>
#include <ap_fixed.h>

void kernel_cholesky(int n, float A[120][120])
{
    #pragma HLS dataflow
    #pragma HLS array_partition variable=A cyclic factor=4

    int i;
    int j;
    int k;

    for (i = 0; i < 120; i++) {
        #pragma HLS loop_flatten
        #pragma HLS pipeline II=1

        for (j = 0; j < i; j++) {
            for (k = 0; k < j; k++) {
                A[i][j] -= A[i][k] * A[j][k];
            }
            A[i][j] /= A[j][j];
        }

        // i==j case
        for (k = 0; k < i; k++) {
            A[i][i] -= A[i][k] * A[i][k];
        }
        A[i][i] = sqrt(A[i][i]);
    }
}
```

In this optimized code, the `#pragma HLS dataflow` directive enables dataflow execution, and the `#pragma HLS array_partition` directive partitions the array `A` into smaller chunks. The `#pragma HLS loop_flatten` directive flattens the nested loops into a single loop, and the `#pragma HLS pipeline II=1` directive enables pipelining with a pipeline depth of 1.