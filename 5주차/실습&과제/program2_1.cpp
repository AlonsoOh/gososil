#include "my_solver.h"

double getIntegral(double start, double end, int count, double dis, double** arr) {
	double ans=0;

	for (int i = 0; i < count-1; i++) 
		if (arr[0][i] >= start && arr[0][i] <= end) 
			ans += (dis /2 * (arr[1][i] + arr[1][i + 1]));
		
	return ans;
}

void program2_1() {
	FILE* fp_r, *fp_w;
	__int64 start, freq, end;
	float resultTime = 0;

	fp_r = fopen("sampling_table.txt", "r");

	if (fp_r == NULL) {
		printf("input file not found....\n");
		exit(0);
	}

	fp_w = fopen("pdf_table.txt", "w");

	/***************************************************/
	int count;
	double dis;
	double integral=0;
	double** sampleNum;
	double** result;

	fscanf(fp_r, "%d %lf", &count, &dis);
	dis = (double)1 / count;
	sampleNum = (double**)malloc(sizeof(double*) * 2);
	result = (double**)malloc(sizeof(double*) * 2);
	sampleNum[0] = (double*)malloc(sizeof(double) * count);
	sampleNum[1] = (double*)malloc(sizeof(double) * count);
	result[0] = (double*)malloc(sizeof(double) * count);
	result[1] = (double*)malloc(sizeof(double) * count);

	for (int i = 0; i < count; i++) 
		fscanf(fp_r, "%lf %lf", &sampleNum[0][i], &sampleNum[1][i]);
	
	for (int i = 0; i < count-1; i++) 
		integral += dis /2 * (sampleNum[1][i] + sampleNum[1][i + 1]);
	
	for (int i = 0; i < count; i++) {
		result[0][i] = (sampleNum[0][i] - sampleNum[0][0]) / (sampleNum[0][count - 1] - sampleNum[0][0]);
		result[1][i] = sampleNum[1][i] / integral;
	}

	fprintf(fp_w, "%d %lf\n", count, result[0][1]-result[0][0]);

	for(int i=0;i<count;i++)
		fprintf(fp_w,"%lf %lf\n", result[0][i], result[1][i]);
	
	printf("*** Integrating the pdf from 0.0 to 1.0 = %lf\n", getIntegral(0.0, 1.0, count, dis, result));
	printf("*** Integrating the pdf from 0.0 to 0.25 = %lf\n", getIntegral(0.0, 0.25, count, dis, result));
	printf("*** Integrating the pdf from 0.25 to 0.5 = %lf\n", getIntegral(0.25, 0.5, count, dis, result));
	printf("*** Integrating the pdf from 0.5 to 0.75 = %lf\n", getIntegral(0.5, 0.75, count, dis, result));
	printf("*** Integrating the pdf from 0.75 to 1.0 = %lf\n", getIntegral(0.75, 1.0, count, dis, result));

	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	
	if (fp_w != NULL) fclose(fp_w);
}
