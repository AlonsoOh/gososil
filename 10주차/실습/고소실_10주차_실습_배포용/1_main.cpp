#include <stdio.h>
#include <string.h>
#include <random>
#include <time.h>

#include <math.h>
#include <time.h>
#include <Windows.h>

__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;


#define MATDIM 1024
double *pMatA, *pMatB, *pMatC;
void MultiplySquareMatrices_1(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_2(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_3(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_4(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);

void init_MatMat(void);

void main(void) {
	init_MatMat();
	CHECK_TIME_START;
	MultiplySquareMatrices_1(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);

	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_1 : %f ms\n", compute_time);
	CHECK_TIME_START;
	MultiplySquareMatrices_2(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);

	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_2 = %f ms\n", compute_time);
	CHECK_TIME_START;
	MultiplySquareMatrices_3(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);

	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_3 = %f ms\n", compute_time);
	CHECK_TIME_START;
	MultiplySquareMatrices_4(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);

	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_4 = %f ms\n", compute_time);
}

void MultiplySquareMatrices_1(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize) {
	int i, j, k;

	memset(pDestMatrix, 0, sizeof(double) * MatSize * MatSize);

	for (i = 0; i < MatSize; i++) 
		for (j = 0; j < MatSize; j++) 
			for (k = 0; k < MatSize; k++) 
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k] * pRightMatrix[k * MatSize + j];

}
void MultiplySquareMatrices_2(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize) {
	int i, j, k;

	memset(pDestMatrix, 0, sizeof(double) * MatSize * MatSize);

	for (k = 0; k < MatSize; k++) 
		for (i = 0; i < MatSize; i++) 
			for (j = 0; j < MatSize; j++) 
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k] * pRightMatrix[k * MatSize + j];
	
}
void MultiplySquareMatrices_3(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize) {
	int i, j, k;
	int unroll = MatSize / 8;
	
	memset(pDestMatrix, 0, sizeof(double) * MatSize * MatSize);

	for (k = 0; k < unroll; k++) {
		for (i = 0; i < MatSize; i++) {
			for (j = 0; j < MatSize; j++) {
				int x = 8 * k, iMat = i * MatSize;
				double& ret = pDestMatrix[iMat + j];

				ret += pLeftMatrix[iMat + x] * pRightMatrix[(x++) * MatSize + j];
				ret += pLeftMatrix[iMat + x] * pRightMatrix[(x++) * MatSize + j];
				ret += pLeftMatrix[iMat + x] * pRightMatrix[(x++) * MatSize + j];
				ret += pLeftMatrix[iMat + x] * pRightMatrix[(x++) * MatSize + j];
				ret += pLeftMatrix[iMat + x] * pRightMatrix[(x++) * MatSize + j];
				ret += pLeftMatrix[iMat + x] * pRightMatrix[(x++) * MatSize + j];
				ret += pLeftMatrix[iMat + x] * pRightMatrix[(x++) * MatSize + j];
				ret += pLeftMatrix[iMat + x] * pRightMatrix[(x++) * MatSize + j];
			}
		}
	}
}
void MultiplySquareMatrices_4(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize) {
	int i, j, k;
	int unroll = MatSize / 2;
	memset(pDestMatrix, 0, sizeof(double) * MatSize * MatSize);
	int x, iMat;

	for (k = 0; k < unroll; k++) {
		for (i = 0; i < MatSize; i++) {
			for (j = 0; j < MatSize; j++) {
				x = 2 * k, iMat = i * MatSize;
				double& ret = pDestMatrix[iMat + j];

				ret += pLeftMatrix[iMat + x] * pRightMatrix[(x++) * MatSize + j];
				ret += pLeftMatrix[iMat + x] * pRightMatrix[(x++) * MatSize + j];
			}
		}
	}
}

void init_MatMat(void) {
	double *ptr;
	pMatA = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	pMatB = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	pMatC = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	srand((unsigned)time(NULL));
	ptr = pMatA;

	for (int i = 0; i < MATDIM*MATDIM; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);

	ptr = pMatB;

	for (int i = 0; i < MATDIM*MATDIM; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);
}
