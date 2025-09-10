void fft(DTYPE X R[SIZE], DTYPE X I[SIZE], DTYPE OUT R[SIZE], DTYPE OUT I[SIZE])
{
#pragma HLS dataflow
DTYPE Stage1 R[SIZE], Stage1 I[SIZE];
DTYPE Stage2 R[SIZE], Stage2 I[SIZE];
DTYPE Stage3 R[SIZE], Stage3 I[SIZE];

#pragma HLS pipeline II=1
bit_reverse(X R, X I, Stage1 R, Stage1 I);
fft_stage_one(Stage1 R, Stage1 I, Stage2 R, Stage2 I);
fft_stages_two(Stage2 R, Stage2 I, Stage3 R, Stage3 I);
fft_stage_three(Stage3 R, Stage3 I, OUT R, OUT I);
}

void bit_reverse(DTYPE X R[SIZE], DTYPE X I[SIZE], DTYPE OUT R[SIZE], DTYPE OUT I[SIZE])
{
#pragma HLS pipeline II=1
for (int i = 0; i < SIZE; i++) {
    OUT R[i] = X R[reverse_bits(i)];
    OUT I[i] = X I[reverse_bits(i)];
}
}

int reverse_bits(int x)
{
    int result = 0;
    for (int i = 0; i < sizeof(int) * 8; i++) {
        result = (result << 1) | (x & 1);
        x >>= 1;
    }
    return result;
}