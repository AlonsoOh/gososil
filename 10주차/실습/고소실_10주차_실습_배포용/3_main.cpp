#include <stdio.h>
#include <random>
#include <time.h>
#include <math.h>
#include <time.h>
#include <Windows.h>

#define N 25
double Taylor_series(double x, int n);
double Taylor_series_ex(double x, int n);
double Taylor_series_ex2(double x, int n);
double factorial(int);
double fastpw(double, int);
__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;

void main(void)
{
	CHECK_TIME_START;
	printf("*** f<-8.3> = %.6e \n", Taylor_series(-8.3, N));
	CHECK_TIME_END(compute_time);
	printf("***Polynomial Naive  compute time : %f ms\n", compute_time);
	
	CHECK_TIME_START;
	printf("*** f<-8.3> = %.6e \n", Taylor_series_ex(-8.3, N));
	CHECK_TIME_END(compute_time);
	printf("***Polynomial ex  compute time : %f ms\n", compute_time);


	CHECK_TIME_START;
	printf("*** f<-8.3> = %.6e \n", Taylor_series_ex2(-8.3, N));
	CHECK_TIME_END(compute_time);
	printf("***Polynomial ex2  compute time : %f ms\n", compute_time);

	CHECK_TIME_START;
	printf("*** f<-8.3> = %.6e \n", pow(2.71828182846, -8.3));
	CHECK_TIME_END(compute_time);
	printf("***Polynomial compute time using pow function : %f ms\n", compute_time);
}

double Taylor_series(double x, int n)
{
	double answer = 0.0f;
	for (int i = 2*n; i >= 1; i--) {
		answer = (answer * x / i) + 1;
	}
	return answer;
}

double factorial(int n) {
	double ret = 1.0f;

	for (int i = 1; i <= n; i++)
		ret *= i;

	return ret;
}
double fastpw(double x, int e) {
	double ret = 1.0;
	while (e) {
		if (e % 2) ret *= x;
		e /= 2, x *= x;
	}
	return ret;
}
double Taylor_series_ex(double x, int n)
{
	double answer = 0.0f;
	answer += 1.0;
	for (int i = 1; i <= 2*n; i++) {

		answer += fastpw(x, i) / factorial(i);
	}
	return answer;
}


double Taylor_series_ex2(double x, int n)
{
	double answer = 0.0f;
	answer += 1.0;
	for (int i = 1; i <= 2 * n; i++) {

		answer += pow(x, i) / factorial(i);
	}
	return answer;
}
