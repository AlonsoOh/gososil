#include "my_solver.h"

void practice3_2() {
	char readfile[256];
	char writefile[256];

	int NCOEF, DEGREE;

	double* poly;
	double* zeror, * zeroi;
	long int fail;
	int i;

	for (int t = 1; t <= 6; t++) {

		sprintf(readfile, "polynomial_3-2_%d.txt", t);
		sprintf(writefile, "roots_3-2_%d.txt", t);

		FILE* fp_r = fopen(readfile, "r");
		if (fp_r == NULL) {
			printf("%s file open error...\n", readfile);
			return;
		}

		FILE* fp_w = fopen(writefile, "w");
		if (fp_w == NULL) {
			printf("%s file open error...\n", writefile);
			return;
		}

		fscanf(fp_r, "%d", &DEGREE);
		NCOEF = DEGREE + 1;

		poly = (double*)malloc(sizeof(double) * NCOEF);
		zeror = (double*)malloc(sizeof(double) * DEGREE);
		zeroi = (double*)malloc(sizeof(double) * DEGREE);


		/********************************/
		fail = 0;
		for (int i = 0; i < NCOEF; i++) fscanf(fp_r, "%lf", &poly[i]);
		rpoly_(poly, &DEGREE, zeror, zeroi, &fail);
		/*for (int i = 0; i < NCOEF; i++) printf("%lf ", poly[i]);
		puts("");*/
		if (fail) {
			printf("failed practice3_2 at %d\n", t);
			continue;
		}
		
		for (int i = 0; i < DEGREE; i++)
			fprintf(fp_w, "%.10lf ", zeror[i]);
		fprintf(fp_w, "\n");
		for (int i = 0; i < DEGREE; i++)
			fprintf(fp_w, "%.10lf ", zeroi[i]);
		fprintf(fp_w, "\n");
		fprintf(fp_w, "function below:\n");
		for (int i = 0; i < DEGREE; i++) {
			if (abs(zeroi[i]) < 1e-9) {
				double sum = 0;
				for (int j = 0; j < NCOEF; j++)
					sum += poly[j] * pow(zeror[i], DEGREE - j);
				fprintf(fp_w, "|f(%.10lf)| for %dth = %.10lf\n", zeror[i], i, abs(sum));
			}
		}
		/********************************/


		free(zeroi);
		free(zeror);
		free(poly);

		if (fp_r != NULL) fclose(fp_r);
		if (fp_w != NULL) fclose(fp_w);
	}
}