#include "my_solver.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

int count1, randNum;
double dis1;
double** px1;
double* randArr;

double func1(double x, double u) {
	double ans = 0;
	int idx = 0;

	for (int i = 0; i < count1 - 1; i++) {
		if (px1[0][i] < x) {
			ans += (dis1 / 2 * (px1[1][i] + px1[1][i + 1]));
			idx = i;
		}
	}

	ans += (px1[1][idx] + ((px1[1][idx + 1] - px1[1][idx]) / (px1[0][idx + 1] - px1[0][idx])) * ((x - px1[0][idx]) / 2)) * (x - px1[0][idx]);
	return ans - u;
}

double diffFunc(double x) {
	int idx = 0;

	for (int i = 0; i < count1-1; i++) {
		if (px1[0][i] <= x && px1[0][i + 1] >= x) {
			idx = i;
			break;
		}
	}

	double s = (x - px1[0][idx]) / (px1[0][idx + 1] - px1[0][idx]);

	return (1 - s) * px1[1][idx] + s * px1[1][idx + 1];
}

double biSection1(double u, int flag) {
	double a0 = 0; double b0 = 1;
	double mid, before;
	int i = 0;

	mid = (a0 + b0) / 2;

	if (flag > 0) i = Nmax - flag;
	
	while (i<=Nmax) {
		if (func1(mid, u) < 0) {
			if (func1(a0, u) < 0) a0 = mid;
			
			else  b0 = mid;
		}

		else {
			if (func1(a0, u) < 0) b0 = mid;
			
			else  a0 = mid;
		}

		double before = mid;
		
		mid = (a0 + b0) / 2;
		i++;

		if (fabs(func1(mid, u)) < DELTA) break;
		//if (i >= Nmax) break;
		if (fabs(mid - before) < EPSILON) break;
	}

	return mid;
}

double secant(double u) {
	double x1 = 1; double x0 = 0;
	int i = 0;
	double x2;

	while (1) {
		x2 = x1 - func1(x1, u) * ((x1 - x0) / (func1(x1, u) - func1(x0, u)));
		i++;

		if (fabs(func1(x2, u)) < DELTA) break;
		
		if (i >= Nmax) break;
		
		if (fabs(x2 - x1) < EPSILON) break;

		x0 = x1;
		x1 = x2;
	}

	return x2;
}

double newton(double u) {
	double x0 = biSection1(u, 3);
	double x1;
	int i = 0;

	while (1) {
		x1 = x0 - func1(x0, u) / diffFunc(x0);
		i++;

		if (fabs(func1(x1, u)) < DELTA) break;
		
		if (i >= Nmax) break;
		
		if (fabs(x1 - x0) < EPSILON) break;

		x0 = x1;
	}

	return x1;
}

void expoRand() {
	int num;
	
	printf("Enter n for exponential distribution: ");
	scanf("%d", &num);

	for (int i = 0; i < 3; i++) {
		double irand, ramda;
		double sum = 0; double squareSum = 0;
		unsigned int iseed = (unsigned int)time(NULL);

		printf("Enter ramda for test%d: ", i);
		scanf("%lf", &ramda);
		srand(iseed);
		
		for (int j = 0; j < num; j++) {
			irand = (double)rand() / 32767;
			double x = -(log(1 - irand) / ramda);
			
			if (x == INFINITY) {
				j--; 
				continue;
			}
			sum += x; squareSum += x * x;
		}

		if (sum / num > 2) printf("%lf %lf\n", sum, num);
		
		printf("Average: %lf\n", sum / num);
		printf("Variance: %lf\n", squareSum / num - (sum / num) * (sum / num));
	}
}

void getTime() {
	printf("\nEnter n for time check: ");
	scanf("%d", &randNum);
	randArr = (double*)malloc(sizeof(double) * randNum);

	int i = 0; double irand;
	unsigned int iseed = (unsigned int)time(NULL);
	
	srand(iseed);
	
	while (i < randNum) {
		irand = (double)rand() / 32767;
		randArr[i] = irand;
		i++;
	}

	// Homework 2-2
	program2_2_a();
	program2_2_b();
}



// HOMEWORK
//통계값과 이론값 구하기
void program2_3() {
	FILE* fp_r, * fp_w;
	FILE* fp_rand;
	
	int size;
	double* randTable;
	double divideNum=0;
	int* divided;

	fp_r = fopen("pdf_table.txt", "r");
	fp_rand = fopen("random_event_table.txt", "r");
	fp_w = fopen("histogram.txt", "w");
	fscanf(fp_r, "%d %lf", &count1, &dis1);
	px1 = (double**)malloc(sizeof(double*) * 2);
	px1[0] = (double*)malloc(sizeof(double) * count1);
	px1[1] = (double*)malloc(sizeof(double) * count1);
	divided = (int*)malloc(sizeof(int) * count1);

	for (int i = 0; i < count1; i++) {
		divided[i] = 0;
		fscanf(fp_r, "%lf %lf", &px1[0][i], &px1[1][i]);
	}

	fscanf(fp_rand, "%d", &size);
	randTable = (double*)malloc(sizeof(double) * size);

	for (int i = 0; i < size; i++) {
		fscanf(fp_rand, "%lf", &randTable[i]);
		divided[(int)(randTable[i] / dis1)]++;
	}

	fprintf(fp_w, "%lf %lf\n", count1, dis1);
	
	double start = 0;
	
	for (int i = 0; i < count1; i++) {
		fprintf(fp_w, "%lf %lf %d\n", start, start + dis1, divided[i]);
		start += dis1;
	}
	/***************************************************/
	if (fp_r != NULL) fclose(fp_r);
	
	if (fp_w != NULL) fclose(fp_w);

	expoRand(); // Homework 2-1
	getTime(); // Homework 2-2
	free(px1[0]); free(px1[1]);
	free(px1); free(randArr); free(randTable);
}

// HOMEWORK
// 난수 생성 프로그램 최적화
void program2_2_a() {
	__int64 start, freq, end;
	float resultTime = 0;

	CHECK_TIME_START;
	// something to do...
	for (int i = 0; i < randNum; i++) 
		double result = biSection1(randArr[i], 0);
		//printf("%.15lf\n", result);
	
	CHECK_TIME_END(resultTime);
	printf("The program2_2_a run time is %f(ms)..\n", resultTime * 1000.0);
}

//bisection이 아닌 다른 방법으로 난수 생성하기
void program2_2_b() {
	__int64 start, freq, end;
	float resultTime = 0;

	CHECK_TIME_START;
	// something to do...
	for (int i = 0; i < randNum; i++) 
		//double result = secant(randArr[i]);
		double result = newton(randArr[i]);
		//printf("%.15lf\n", result);
	

	CHECK_TIME_END(resultTime);
	printf("The program2_2_b run time is %f(ms)..\n", resultTime * 1000.0);
}