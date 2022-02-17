#pragma once

#include "cuda_code.cuh"

#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

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

void create_device_timer()
{
	CUDA_CALL(cudaEventCreate(&cuda_timer_start));
	CUDA_CALL(cudaEventCreate(&cuda_timer_stop));
}

void destroy_device_timer()
{
	CUDA_CALL(cudaEventDestroy(cuda_timer_start));
	CUDA_CALL(cudaEventDestroy(cuda_timer_stop));
}

inline void start_device_timer()
{
	cudaEventRecord(cuda_timer_start, CUDA_STREAM_0);
}

inline TIMER_T stop_device_timer()
{
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

TIMER_T compute_time = 0;
TIMER_T device_time = 0;

#define Window 2

__constant__ float constant_gaussian_kernel[25];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Gaussian ���͸��� �ϴ� Ŀ��
//	shared memory�� ������� �ʴ´�
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BLOCK_SIZE (1 << 4)													// CUDA Ŀ�� thread block ������
__global__ void Gaussian_kernel_no_shared(IN unsigned char* d_bitmaps, OUT unsigned char* d_Gaussian, long width, long height) {
	const unsigned row = blockIdx.y * blockDim.y + threadIdx.y;
	const unsigned col = blockIdx.x * blockDim.x + threadIdx.x;
	const unsigned id = row * width + col;

	int r = row, c = col;
	int w0 = 5;
	int w = floor((double)w0 / 2);
	int k, l;
	double temp, mean;

	mean = 0; d_Gaussian[id] = 0;
	for (l = -w; l <= w; l++) {
		for (k = -w; k <= w; k++) {
			if ((r + l) < 0 || (r + l) >= height || (c + k) < 0 || (c + k) >= width) continue;
			else
				mean += d_bitmaps[(r + l) * width + c + k] * constant_gaussian_kernel[(l + w) * w0 + k + w];
		}
	}
	d_Gaussian[id] = mean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Gaussian ���͸��� �ϴ� Ŀ��
//	shared memory�� ����Ѵ�.
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern __shared__ unsigned char sharedBuffer[];
__global__ void Gaussian_kernel_shared(INOUT unsigned char* d_bitmaps, OUT unsigned char* d_Gaussian, long width, long height) {
	const unsigned row = blockIdx.y * blockDim.y + threadIdx.y;
	const unsigned col = blockIdx.x * blockDim.x + threadIdx.x;
	const unsigned id = row * width + col;

	int r = row, c = col;
	int w0 = 5;
	int w = floor((double)w0 / 2);
	int k, l;
	double temp, mean;

	d_Gaussian[id] = 0;
	int thread_id = (threadIdx.y + w) * (BLOCK_SIZE + 2 * w) + (threadIdx.x + w);
	sharedBuffer[thread_id] = d_bitmaps[id];
	int x;
	if (threadIdx.x < w) {
		for (l = threadIdx.y - w; l < BLOCK_SIZE + 2 * w; l++) {
			for (k = threadIdx.x - w; k < 0; k++) {
				x = (l + w) * w + k + w;
				if ((r + l) < 0 || (r + l) >= height || (c + k) < 0 || (c + k) >= width) continue;
				else
					sharedBuffer[x] = d_bitmaps[(r + l) * width + c + k];
			}
		}
	}
	else if (threadIdx.x == BLOCK_SIZE - 1) {
		for (l = threadIdx.y - w; l < BLOCK_SIZE + 2 * w; l++) {
			for (k = threadIdx.x + 1; k < BLOCK_SIZE + w; k++) {
				x = (l + w) * w + k - 1;
				if ((r + l) < 0 || (r + l) >= height || (c + k) < 0 || (c + k) >= width) continue;
				else
					sharedBuffer[x] = d_bitmaps[(r + l) * width + c + k];
			}
		}
	}
	if (threadIdx.y < w) {
		for (l = threadIdx.y - w; l < 0; l++) {
			for (k = threadIdx.x - w; k < BLOCK_SIZE + 4; k++) {
				x = (l + w) * (BLOCK_SIZE + 2 * w) + k + w;
				if ((r + l) < 0 || (r + l) >= height || (c + k) < 0 || (c + k) >= width) continue;
				else
					sharedBuffer[x] = d_bitmaps[(r + l) * width + c + k];
			}
		}
	}
	else if (threadIdx.y == BLOCK_SIZE - 1) {
		for (l = threadIdx.y + 1; l < BLOCK_SIZE + w; l++) {
			for (k = threadIdx.x - w; k < BLOCK_SIZE + 2 * w; k++) {
				x = (l - 1) * (BLOCK_SIZE + 2 * w) + k + w;
				if ((r + l) < 0 || (r + l) >= height || (c + k) < 0 || (c + k) >= width) continue;
				else
					sharedBuffer[x] = d_bitmaps[(r + l) * width + c + k];
			}
		}
	}
	__syncthreads();
	mean = 0;
	for (k = -w; k <= w; k++) {
		for (l = -w; l <= w; l++) {
			mean += sharedBuffer[(threadIdx.y + 2 + k) * (BLOCK_SIZE + 4) + (threadIdx.x + 2 + l)] * constant_gaussian_kernel[(k + w) * w0 + w + l];
		}
	}

	d_Gaussian[id] = mean;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Constant variable �� gaussian kernel�� �����ϴ� �Լ�
//	�Ŀ� gaussian filtering ���� ����Ѵ�.
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Set_Gaussain_Kernel() {
	float _1 = 1.0f / 256.0f;
	float _4 = _1 * 4;
	float _6 = _1 * 6;
	float _16 = _1 * 16;
	float _24 = _1 * 24;
	float _36 = _1 * 36;

	float* p_gaussian_kernel = new float[25];

	p_gaussian_kernel[0] = p_gaussian_kernel[4] = p_gaussian_kernel[20] = p_gaussian_kernel[24] = _1;
	p_gaussian_kernel[1] = p_gaussian_kernel[3] = p_gaussian_kernel[5] = p_gaussian_kernel[9] = _4;
	p_gaussian_kernel[15] = p_gaussian_kernel[19] = p_gaussian_kernel[21] = p_gaussian_kernel[23] = _4;
	p_gaussian_kernel[2] = p_gaussian_kernel[10] = p_gaussian_kernel[14] = p_gaussian_kernel[22] = _6;
	p_gaussian_kernel[6] = p_gaussian_kernel[8] = p_gaussian_kernel[16] = p_gaussian_kernel[18] = _16;
	p_gaussian_kernel[7] = p_gaussian_kernel[11] = p_gaussian_kernel[13] = p_gaussian_kernel[17] = _24;
	p_gaussian_kernel[12] = _36;

	cudaMemcpyToSymbol(constant_gaussian_kernel, p_gaussian_kernel, sizeof(float) * 25);

	delete[] p_gaussian_kernel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Ŀ���� �����ϱ� �� �ʿ��� �ڷ�� �غ� �� Ŀ���� ������ ����̽��� ����
//	Shared_flag �Է� �� NO_SHARED �� SHARED �� �� ���� ��ũ�θ� ������
//	flag���� �´� Ŀ���� ����
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Do_Gaussian_on_GPU(IN unsigned char* p_bitmaps, OUT unsigned char* p_Gaussian, long width, long height, int Shared_flag)
{
	Set_Gaussain_Kernel();
	CUDA_CALL(cudaSetDevice(0));
	unsigned int total_pixel = width * height;

	unsigned char* d_bitmaps, * d_Gaussian;
	size_t mem_size;

	mem_size = width * height * sizeof(unsigned char);
	CUDA_CALL(cudaMalloc(&d_bitmaps, mem_size));

	CUDA_CALL(cudaMemcpy(d_bitmaps, p_bitmaps, mem_size, cudaMemcpyHostToDevice));

	CUDA_CALL(cudaMalloc(&d_Gaussian, mem_size));

	dim3 blockDim(BLOCK_SIZE, BLOCK_SIZE);

	dim3 gridDim((width) / blockDim.x, (height) / blockDim.y);
	CHECK_TIME_INIT_GPU();
	CHECK_TIME_START_GPU();

	switch (Shared_flag)
	{
	case NO_SHARED:
		Gaussian_kernel_no_shared << <gridDim, blockDim >> > (d_bitmaps, d_Gaussian, width, height);
		break;
	case SHARED:
		Gaussian_kernel_shared << < gridDim, blockDim, sizeof(unsigned char)* (BLOCK_SIZE + 2 * Window)* (BLOCK_SIZE + 2 * Window) >> > (d_bitmaps, d_Gaussian, width, height);
		break;
	}

	CUDA_CALL(cudaDeviceSynchronize());
	CHECK_TIME_END_GPU(device_time);
	CHECK_TIME_DEST_GPU();

	CUDA_CALL(cudaMemcpy(p_Gaussian, d_Gaussian, mem_size, cudaMemcpyDeviceToHost));

	cudaFree(d_bitmaps);
	cudaFree(d_Gaussian);

	return device_time;
}