#include "my_solver.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

int count;
double dis;
double** px;

double func(double x, double u) {
	double ans = 0;
	int idx=0;

	for (int i = 0; i < count - 1; i++) {
		if (px[0][i] < x) {
			ans += (dis / 2 * (px[1][i] + px[1][i + 1]));
			idx = i;
		}
	}
	//printf("%lf\n", ans - u);
	ans += (px[1][idx] + ((px[1][idx + 1] - px[1][idx]) / (px[0][idx + 1] - px[0][idx])) * ((x - px[0][idx]) / 2)) * (x - px[0][idx]);
	return ans - u;
}

double biSection(double u) {
	double a0 = 0; double b0 = 1;
	double mid, before;
	int i = 0;

	mid = (a0 + b0) / 2;

	while (1) {
		if (func(mid, u) < 0) {
			if (func(a0, u) < 0) a0 = mid;

			else  b0 = mid;
		}

		else {
			if (func(a0,u) < 0) b0 = mid;

			else  a0 = mid;
		}

		double before = mid;

		mid = (a0 + b0) / 2;
		i++;

		if (fabs(func(mid, u)) < DELTA) break;

		if (i >= Nmax) break;

		if (fabs(mid - before) < EPSILON) break;
	}

	return mid;
}

void program2_2() {
	FILE* fp_r, *fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table.txt", "w");

	/***************************************************/
	int m;

	fscanf(fp_r, "%d %lf", &count, &dis);
	px = (double**)malloc(sizeof(double*) * 2);
	px[0] = (double*)malloc(sizeof(double) * count);
	px[1] = (double*)malloc(sizeof(double) * count);

	for (int i = 0; i < count; i++) 
		fscanf(fp_r, "%lf %lf", &px[0][i], &px[1][i]);
	
	printf("\nEnter n: ");
	scanf("%d", &m);
	fprintf(fp_w, "%d\n", m);

	int i = 0; double irand;
	unsigned int iseed = (unsigned int)time(NULL);

	srand(iseed);

	while (i < m) {
		irand = (double)rand()/32767;
		double result = biSection(irand);
		//printf("%lf %.15lf\n",irand, result);
		fprintf(fp_w, "%.15lf\n",result);
		i++;
	}

	free(px[0]); free(px[1]);
	free(px);
	/***************************************************/
	if (fp_r != NULL) fclose(fp_r);

	if (fp_w != NULL) fclose(fp_w);
}
