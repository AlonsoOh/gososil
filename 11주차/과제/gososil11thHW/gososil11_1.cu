#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

#define CUDA_CALL(x) { const cudaError_t a = (x); if(a != cudaSuccess) { printf("\nCuda Error: %s (err_num=%d) at line:%d\n", cudaGetErrorString(a), a, __LINE__); cudaDeviceReset(); assert(0);}}
typedef float TIMER_T;

#define USE_CPU_TIMER 1
#define USE_GPU_TIMER 1

#if USE_CPU_TIMER == 1
__int64 start, freq, end;
#define CHECK_TIME_START { QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start); }
#define CHECK_TIME_END(a) { QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f)); }
#else
#define CHECK_TIME_START
#define CHECK_TIME_END(a)
#endif

#if USE_GPU_TIMER == 1
cudaEvent_t cuda_timer_start, cuda_timer_stop;
#define CUDA_STREAM_0 (0)

void create_device_timer() {
    CUDA_CALL(cudaEventCreate(&cuda_timer_start));
    CUDA_CALL(cudaEventCreate(&cuda_timer_stop));
}

void destroy_device_timer() {
    CUDA_CALL(cudaEventDestroy(cuda_timer_start));
    CUDA_CALL(cudaEventDestroy(cuda_timer_stop));
}

inline void start_device_timer() {
    cudaEventRecord(cuda_timer_start, CUDA_STREAM_0);
}

inline TIMER_T stop_device_timer() {
    TIMER_T ms;
    cudaEventRecord(cuda_timer_stop, CUDA_STREAM_0);
    cudaEventSynchronize(cuda_timer_stop);
    cudaEventElapsedTime(&ms, cuda_timer_start, cuda_timer_stop);
    return ms;
}

#define CHECK_TIME_INIT_GPU() { create_device_timer(); }
#define CHECK_TIME_START_GPU() { start_device_timer(); }
#define CHECK_TIME_END_GPU(a) { a = stop_device_timer(); }
#define CHECK_TIME_DEST_GPU() { destroy_device_timer(); }
#else
#define CHECK_TIME_INIT_GPU()
#define CHECK_TIME_START_GPU()
#define CHECK_TIME_END_GPU(a)
#define CHECK_TIME_DEST_GPU()
#endif

#define BLOCK_SIZE 32
#define N_EQUATIONS (1 << 13)

TIMER_T compute_time = 0;
TIMER_T device_time = 0;

int N;
float* A, * B, * C;
float* X0, * X1, * FX0, * FX1;
float* X0_GPU, * X1_GPU, * FX0_GPU, * FX1_GPU;

cudaError_t find_roots_GPU(float* A, float* B, float* C,
    float* X0, float* X1, float* FX0, float* FX1, int n);

__global__ void find_roots_Kernel(float* A, float* B, float* C,
    float* X0, float* X1, float* FX0, float* FX1, int n) {
    /*Thread Idx*/
    int row;
    int col;
    int tid;
    /*Todo*/
    row = blockDim.y * blockIdx.y + threadIdx.y;
    tid = col = blockDim.x * blockIdx.x + threadIdx.x;
    float a, b, c, d, x0, x1, tmp;

    a = A[tid]; b = B[tid]; c = C[tid];
    d = sqrtf(b * b - 4.0f * a * c);
    tmp = 1.0f / (2.0f * a);
    x0 = (-b - d) * tmp;
    x1 = (-b + d) * tmp;

    if (x0 > x1) {
        int temp = x1;
        x1 = x0;
        x0 = temp;
    }

    X0[tid] = x0; X1[tid] = x1;
    FX0[tid] = (a * x0 + b) * x0 + c;
    FX1[tid] = (a * x1 + b) * x1 + c;
}


void find_roots_CPU(float* A, float* B, float* C,
    float* X0, float* X1, float* FX0, float* FX1, int n) {
    int i;
    float a, b, c, d, x0, x1, tmp;

    for (i = 0; i < n; i++) {
        a = A[i]; b = B[i]; c = C[i];
        d = sqrtf(b * b - 4.0f * a * c);
        tmp = 1.0f / (2.0f * a);
        x0 = (-b - d) * tmp;
        x1 = (-b + d) * tmp;

        if (x0 > x1) {
            int temp = x1;
            x1 = x0;
            x0 = temp;
        }

        X0[i] = x0; X1[i] = x1;
        FX0[i] = (a * x0 + b) * x0 + c;
        FX1[i] = (a * x1 + b) * x1 + c;
    }
}

void init_bin_file(int size) {
    srand((unsigned)time(NULL));
    printf("***Binary File init Start!!\n");

    FILE* fp = fopen("A.bin", "wb");
    fwrite(&size, sizeof(int), 1, fp);
    float random_num;

    for (int i = 0; i < size; i++) {
        random_num = (float)(((double)rand() / RAND_MAX) * 40.0f);
        fwrite(&random_num, sizeof(float), 1, fp);
    }
    fclose(fp);
    fp = fopen("B.bin", "wb");
    fwrite(&size, sizeof(int), 1, fp);

    for (int i = 0; i < size; i++) {
        random_num = (float)(((double)rand() / RAND_MAX) * 40.0f);
        fwrite(&random_num, sizeof(float), 1, fp);
    }
    fclose(fp);
    fp = fopen("C.bin", "wb");
    fwrite(&size, sizeof(int), 1, fp);

    for (int i = 0; i < size; i++) {
        random_num = (float)(((double)rand() / RAND_MAX) * 40.0f);
        fwrite(&random_num, sizeof(float), 1, fp);
    }
    fclose(fp);
    printf("***Binary File init End!!\n\n");
}

void read_bin_file() {
    printf("***Binary File Read Start!!\n");
    /*Todo*/
    A = new float[N_EQUATIONS];
    B = new float[N_EQUATIONS];
    C = new float[N_EQUATIONS];
    X0 = new float[N_EQUATIONS];
    X1 = new float[N_EQUATIONS];
    FX0 = new float[N_EQUATIONS];
    FX1 = new float[N_EQUATIONS];
    X0_GPU = new float[N_EQUATIONS];
    X1_GPU = new float[N_EQUATIONS];
    FX0_GPU = new float[N_EQUATIONS];
    FX1_GPU = new float[N_EQUATIONS];

    FILE* fp = fopen("A.bin", "rb");

    fread(&N, sizeof(int), 1, fp);
    fread(A, sizeof(float), N_EQUATIONS, fp);
    fclose(fp);
    fp = fopen("B.bin", "rb");
    fread(&N, sizeof(int), 1, fp);
    fread(B, sizeof(float), N_EQUATIONS, fp);
    fclose(fp);
    fp = fopen("C.bin", "rb");
    fread(&N, sizeof(int), 1, fp);
    fread(C, sizeof(float), N_EQUATIONS, fp);
    fclose(fp);
    printf("***Binary File Read End!!\n\n");
}

int main() {
    init_bin_file(N_EQUATIONS);
    read_bin_file();
    printf("The problem size is %d.\n", N);
    int i;
    FILE* fp;

    //CPU 
    printf("***Fibonacci_CPU Start!!\n");
    CHECK_TIME_START;
    find_roots_CPU(A, B, C, X0, X1, FX0, FX1, N);
    CHECK_TIME_END(compute_time);
    printf("***Fibonacci_CPU End!!\n\n");
    printf("CPU time = %.6f\n", compute_time);
    //GPU

    printf("***Fibonacci_GPU Start!!\n");
    find_roots_GPU(A, B, C, X0, X1, FX0_GPU, FX1_GPU, N);
    printf("***Fibonacci_GPU End!!\n\n");
    printf("GPU time = %.6f\n", device_time);

    for (i = 0; i < N; i++) {
        if (FX0[i] != FX0_GPU[i] && !isnan(FX0[i]) && !isnan(FX0_GPU[i])) {
            printf("%f != %f\n", FX0[i], FX0_GPU[i]);
            break;
        }

        if (FX1[i] != FX1_GPU[i] && !isnan(FX1[i]) && !isnan(FX1_GPU[i])) {
            printf("%f != %f\n", FX1[i], FX1_GPU[i]);
            break;
        }
    }

    if (i == N) printf("***Kernel execution Success!!\n\n");

    //  Write the output array into the output file.
    fp = fopen("X0.bin", "wb");

    if (!fp) {
        fprintf(stderr, "Error: cannot open the output file...\n");
        exit(-1);
    }
    fwrite(&N, sizeof(int), 1, fp);

    for (i = 0; i < N; i++)
        fwrite(&X0_GPU[i], sizeof(float), 1, fp);

    fclose(fp);

    fp = fopen("X1.bin", "wb");

    if (!fp) {
        fprintf(stderr, "Error: cannot open the output file...\n");
        exit(-1);
    }
    fwrite(&N, sizeof(int), 1, fp);

    for (i = 0; i < N; i++)
        fwrite(&X1_GPU[i], sizeof(float), 1, fp);
    fclose(fp);

    fp = fopen("FX0.bin", "wb");

    if (!fp) {
        fprintf(stderr, "Error: cannot open the output file...\n");
        exit(-1);
    }
    fwrite(&N, sizeof(int), 1, fp);

    for (i = 0; i < N; i++)
        fwrite(&FX0_GPU[i], sizeof(float), 1, fp);

    fclose(fp);
    fp = fopen("FX1.bin", "wb");

    if (!fp) {
        fprintf(stderr, "Error: cannot open the output file...\n");
        exit(-1);
    }
    fwrite(&N, sizeof(int), 1, fp);

    for (i = 0; i < N; i++)
        fwrite(&FX1_GPU[i], sizeof(float), 1, fp);

    fclose(fp);
    printf("end!!\n\n");
    return 0;
}


cudaError_t find_roots_GPU(float* A, float* B, float* C,
    float* X0, float* X1, float* FX0, float* FX1, int n) {
    CHECK_TIME_INIT_GPU();

    cudaError_t cudaStatus;
    /*Todo*/
    cudaStatus = cudaSetDevice(0);

    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    cudaDeviceProp deviceProp;
    CUDA_CALL(cudaGetDeviceProperties(&deviceProp, 0));

    float* d_A, * d_B, * d_C;
    float* d_X0, * d_X1, * d_FX0, * d_FX1;
    float* d_X0_GPU, * d_X1_GPU, * d_FX0_GPU, * d_FX1_GPU;
    size_t size = sizeof(float) * N;

    CUDA_CALL(cudaMalloc(&d_A, size))
        CUDA_CALL(cudaMalloc(&d_B, size))
        CUDA_CALL(cudaMalloc(&d_C, size))
        CUDA_CALL(cudaMalloc(&d_X0, size))
        CUDA_CALL(cudaMalloc(&d_X1, size))
        CUDA_CALL(cudaMalloc(&d_FX0, size))
        CUDA_CALL(cudaMalloc(&d_FX1, size))

        CUDA_CALL(cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice))
        CUDA_CALL(cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice))
        CUDA_CALL(cudaMemcpy(d_C, C, size, cudaMemcpyHostToDevice))

        dim3 dimBlock(BLOCK_SIZE);
    dim3 dimGrid(N_EQUATIONS / dimBlock.x);
    CHECK_TIME_START_GPU();
    find_roots_Kernel << < dimGrid, dimBlock >> > (d_A, d_B, d_C, d_X0, d_X1, d_FX0, d_FX1, N);
    CHECK_TIME_END_GPU(device_time);

    CUDA_CALL(cudaDeviceSynchronize())
        CUDA_CALL(cudaMemcpy(X0_GPU, d_X0, size, cudaMemcpyDeviceToHost))
        CUDA_CALL(cudaMemcpy(X1_GPU, d_X1, size, cudaMemcpyDeviceToHost))
        CUDA_CALL(cudaMemcpy(FX0_GPU, d_FX0, size, cudaMemcpyDeviceToHost))
        CUDA_CALL(cudaMemcpy(FX1_GPU, d_FX1, size, cudaMemcpyDeviceToHost))

        Error:
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    cudaFree(d_X0);
    cudaFree(d_X1);
    cudaFree(d_FX0);
    cudaFree(d_FX1);
    CHECK_TIME_DEST_GPU();
    return cudaStatus;
}