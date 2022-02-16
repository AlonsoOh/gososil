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
float compute_time1, compute_time2;


#define MATDIM 1024
#define HW1_N 100000
float *hw1_x, hw1_E, hw1_var1, hw1_var2;
float hw2_a, hw2_b, hw2_c, hw2_naive_ans[2], hw2_pre_ans[2];

/* hw1 */
void init_hw1(int fixed);
void hw1_calc_e();
void hw1_calc_var1();
void hw1_calc_var2();
/* hw2 */
void hw2_naive();
void hw2_safe();
float hw2_verify(float x);
/* hw3 */
void hw3_1();
void hw3_2();
void hw3_3();
void hw3_4();
void hw3_5();

typedef struct {
	float a[1000];
	float b[1000];
}test;

void main(void) {
	srand((unsigned)time(NULL));

	/* hw1 */
	puts("====== hw1 ======");
	init_hw1(1);
	hw1_calc_e();
	CHECK_TIME_START;
	hw1_calc_var1();
	CHECK_TIME_END(compute_time);
	compute_time1 = compute_time;
	printf("hw1_calc_var1 = %f ms\n", compute_time);
	printf("hw1_calc_var1 value = %.15f\n", hw1_var1);

	CHECK_TIME_START;
	hw1_calc_var2();
	CHECK_TIME_END(compute_time);
	compute_time2 = compute_time;
	printf("hw1_calc_var2 = %f ms\n", compute_time);
	printf("hw1_calc_var2 value = %.15f\n", hw1_var2);
	puts("");
	
	/* hw2 */
	puts("====== hw2 ======");
	printf("a, b, c : ");
	scanf("%f %f %f", &hw2_a, &hw2_b, &hw2_c);
	hw2_naive();
	hw2_safe();
	printf("naive result    : %.15f, %.15f\n", hw2_naive_ans[0], hw2_naive_ans[1]);
	printf("advanced result : %.15f, %.15f\n", hw2_pre_ans[0], hw2_pre_ans[1]);
	puts("");
	printf("Verifying naive ans    : %.15f, %.15f\n", hw2_verify(hw2_naive_ans[0]), hw2_verify(hw2_naive_ans[1]));
	printf("Verifying advanced ans : %.15f, %.15f\n", hw2_verify(hw2_pre_ans[0]), hw2_verify(hw2_pre_ans[1]));
	puts("");

	/* hw3 */
	puts("====== hw3 ======");
	hw3_1();
	hw3_2();
	hw3_3();
	hw3_4();
	hw3_5();
}

void init_hw1(int fixed) {
	float *ptr;
	hw1_x = (float *)malloc(sizeof(float)*HW1_N);

	if (fixed) {
		float tmp = HW1_N;
		
		for (int i = 0; i < HW1_N; i++) {
			if( i & 1) tmp -= 0.0001;

			hw1_x[i] = tmp;
		}
	}

	else {
		srand((unsigned)time(NULL));
		ptr = hw1_x;

		for (int i = 0; i < HW1_N; i++)
			*ptr++ = ((float)rand() / (float)RAND_MAX) * 2;
	}
}

void hw1_calc_e() {
	float sum = 0;

	for (int i = 0; i < HW1_N; i++) 
		sum += hw1_x[i];

	hw1_E = sum / HW1_N;
}

void hw1_calc_var1() {
	float sum_x = 0;
	float sum_xs = 0;

	for (int i = 0; i < HW1_N; i++) {
		sum_x += hw1_x[i];
		sum_xs += hw1_x[i] * hw1_x[i];
	}

	hw1_var1 = ((HW1_N * sum_xs) - (sum_x * sum_x)) / (HW1_N * (HW1_N - 1));
}

void hw1_calc_var2() {
	float sum = 0;

	for (int i = 0; i < HW1_N; i++) 
		sum += (hw1_x[i] - hw1_E) * (hw1_x[i] - hw1_E);
	
	hw1_var2 = sum / (HW1_N - 1);
}


/* hw2 */
void hw2_naive() {
	hw2_naive_ans[0] = (-hw2_b + sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c)) / (2 * hw2_a);
	hw2_naive_ans[1] = (-(hw2_b + sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c))) / (2 * hw2_a);
}

void hw2_safe() {
	if (hw2_b > 0) {
		hw2_pre_ans[0] = ((-2 * hw2_c) / (sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c) + hw2_b));
		hw2_pre_ans[1] = (-hw2_b - sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c)) / (2 * hw2_a);
	}

	else {
		hw2_pre_ans[0] = (sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c) - hw2_b) / (2 * hw2_a);
		hw2_pre_ans[1] = ((4 * hw2_a * hw2_c) / (sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c) - hw2_b)) / (2 * hw2_a);
	}
}
float hw2_verify(float x) {
	return hw2_a * x * x + hw2_b*x + hw2_c;
}

/* hw3 */
float func(float a, float b) {
	float result = a * b;
	return result;
}

void parameter(test t) {
	t.a[1] = 2;
}

void pointer(test* t) {
	t->a[1] = 2;
}

void hw3_1() {
	float a = 0; float d = 1; float temp = 0;
	float b = 1; float c = 2; float g = 3;

	srand((unsigned)time(NULL));
	printf("\n=====Common subexpresstion elimination.=====\n");
	printf("original code\n");

	CHECK_TIME_START;

	for (int i = 0; i < 100000; i++) 
		a = b * c + d * g + d * g;
	
	CHECK_TIME_END(compute_time);
	printf("time: %.15f ms\n\n", compute_time);
	printf("optimize code\n");
	CHECK_TIME_START;
	temp = d * g;

	for (int i = 0; i < 100000; i++) 
		a = b * c + temp + temp;
	
	CHECK_TIME_END(compute_time);
	printf("time: %.15f ms\n\n", compute_time);
}

void hw3_2() {
	float a = 0;

	srand((unsigned)time(NULL));
	printf("\n=====Avoid modulo calculation.=====\n");
	printf("original code\n");
	CHECK_TIME_START;

	for (int i = 0; i < 100000; i++) 
		a = i % 60;
	
	CHECK_TIME_END(compute_time);
	printf("time: %.15f ms\n\n", compute_time);
	printf("optimize code\n");
	CHECK_TIME_START;

	for (int i = 0; i < 100000; i++) {
		if (i == 60) a = 0;
		else a = i;
	}

	CHECK_TIME_END(compute_time);
	printf("time: %.15f ms\n\n", compute_time);
}
void hw3_3() {
	float a = 0;
	srand((unsigned)time(NULL));
	printf("\n=====Avoid loop calculation.=====\n");
	printf("original code\n");
	CHECK_TIME_START;

	for (int i = 0; i < 100000; i++) 
		a += i;
	
	CHECK_TIME_END(compute_time);
	printf("time: %.15f ms\n\n", compute_time);
	printf("optimize code\n");
	CHECK_TIME_START;
	a = 100000 * 0.5 * 100001;
	CHECK_TIME_END(compute_time);
	printf("time: %.15f ms\n\n", compute_time);
}

void hw3_4() {
	float a;

	printf("\n=====Avoid useless function call.=====\n");
	printf("original code\n");
	CHECK_TIME_START;

	for (int i = 0; i < 10000; i++) 
		a = func(3, i);
	
	CHECK_TIME_END(compute_time);
	printf("time: %.15f ms\n\n", compute_time);
	printf("optimize code\n");
	CHECK_TIME_START;

	for (int i = 0; i < 10000; i++) 
		a = 3 * i;
	
	CHECK_TIME_END(compute_time);
	printf("time: %.15f ms\n\n", compute_time);
}

void hw3_5() {
	test t;

	printf("\n=====Use pointer.=====\n");
	printf("original code\n");
	CHECK_TIME_START;

	for (int i = 0; i < 1000; i++) 
		parameter(t);
	
	CHECK_TIME_END(compute_time);
	printf("time: %.15f ms\n\n", compute_time);
	printf("optimize code\n");
	CHECK_TIME_START;

	for (int i = 0; i < 1000; i++) 
		pointer(&t);
	
	CHECK_TIME_END(compute_time);
	printf("time: %.15f ms\n\n", compute_time);
}
