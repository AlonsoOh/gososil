#include "my_solver.h"
#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

double C, b, p[5][4], t[5], tr[5];
// f1 = x_1 + 10 * x_2 + 9;
// f2 = sqrt(5.0) * (x_3 - x_4) - 2 * sqrt(5.0);
// f3 = (x_2 - 2 * x_3)^2 - 9;
// f4 = sqrt(10.0)*(x_1 - x_4, 2)^2 - 2 * sqrt(10.0);
void fcn3_1(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag) {
	if (*iflag == 1) {
		for (int i = 1; i <= 4; i++) {
			fvec[i - 1] = (x[0] - p[i][1]) * (x[0] - p[i][1]) + (x[1] - p[i][2]) * (x[1] - p[i][2])
				+ (x[2] - p[i][3]) * (x[2] - p[i][3]) - (C * (tr[i] + x[3] - t[i])) * (C * (tr[i] + x[3] - t[i]));
		}
	}

	else if (*iflag == 2) {
		for (int i = 0; i < 4; i++)
			fjac[i] = 2.0 * x[0] - 2.0 * p[i + 1][1];
		
		for (int i = 4; i < 8; i++)
			fjac[i] = 2.0 * x[1] - 2.0 * p[i + 1 - 4][2];
		
		for (int i = 8; i < 12; i++)
			fjac[i] = 2.0 * x[2] - 2.0 * p[i + 1 - 8][3];
		
		for (int i = 12; i < 16; i++)
			fjac[i] = 2.0 * C * C * (x[3] + tr[i + 1 - 12] - t[i + 1 - 12]);
	}

	return;
}

void fcn3_1D(int* n, double* x, double* fvec, int* iflag) {
	/********************************/

	for (int i = 1; i <= 4; i++) {
		fvec[i - 1] = (x[0] - p[i][1]) * (x[0] - p[i][1]) + (x[1] - p[i][2]) * (x[1] - p[i][2])
			+ (x[2] - p[i][3]) * (x[2] - p[i][3]) - (C * (tr[i] + x[3] - t[i])) * (C * (tr[i] + x[3] - t[i]));
	}
	/********************************/
}

void program3_2() {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 3030, -4030, 3880, 0.01 };	//need to initilize x0
	double y[SOLNUMS] = { 3030, -4030, 3880, 0.01 };
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;
	//freopen("input.txt", "r", stdin);

	char fread_name[] = "GPS_signal_0.txt";
	char file_name[] = "GPS_position_3-1_0.txt";
	
	for (int i = 0; i < 3; i++) {
		fread_name[11] = i + '0';
		file_name[17] = i + '0';
		FILE* fp_r = fopen(fread_name, "r");
		FILE* fp_w = fopen(file_name, "w");
		
		if (fp_r == NULL) {
	   		printf("%s file open error...\n", fread_name);
			return;
		}

		fscanf(fp_r, "%lf %lf", &C, &b);
		
		for (int i = 0; i < 4; i++)
			fscanf(fp_r, "%lf %lf %lf %lf %lf", &p[i + 1][1], &p[i + 1][2], &p[i + 1][3], &t[i + 1], &tr[i + 1]);
		/*for (int i = 0; i < 4; i++)
			scanf("%lf", &x[i]);*/
		for (int i = 0; i < 4; i++) x[i] = y[i];
		
		x[3] = b;
		hybrj1_(fcn3_1, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
		fprintf(fp_w, "\ninfo = %d\n", info);

		fprintf(fp_w, "root = ");
		
		for (int i = 0; i < n; i++) fprintf(fp_w, "\t%.8lf", x[i]);
		
		fprintf(fp_w, "\nf(x) = ");
		
		for (int i = 0; i < n; i++) fprintf(fp_w, "\t%.8lf", fvec[i]);
		
		fprintf(fp_w, "\n");
		fclose(fp_r);
		fclose(fp_w);
	}

	char file_name2[] = "GPS_position_3-2_0.txt";
	
	for (int i = 0; i < 3; i++) {
		fread_name[11] = i + '0';
		file_name2[17] = i + '0';
		FILE* fp_r = fopen(fread_name, "r");
		FILE* fp_w = fopen(file_name2, "w");
		
		if (fp_r == NULL) {
			printf("%s file open error...\n", fread_name);
			return;
		}
		
		fscanf(fp_r, "%lf %lf", &C, &b);
		
		for (int i = 0; i < 4; i++)
			fscanf(fp_r, "%lf %lf %lf %lf %lf", &p[i + 1][1], &p[i + 1][2], &p[i + 1][3], &t[i + 1], &tr[i + 1]);
		/*for (int i = 0; i < 4; i++)
			scanf("%lf", &x[i]);*/
		
		for (int i = 0; i < 4; i++) x[i] = y[i];
		
		x[3] = b;
		hybrd1_(fcn3_1D, &n, x, fvec, &tol, &info, wa, &lwa);
		fprintf(fp_w, "\ninfo = %d\n", info);
		fprintf(fp_w, "root=");
		
		for (int i = 0; i < n; i++) fprintf(fp_w, "\t%.8lf", x[i]);
		
		fprintf(fp_w, "\nf(x)=");
		
		for (int i = 0; i < n; i++) fprintf(fp_w, "\t%.8lf", fvec[i]);
		
		fprintf(fp_w, "\n");
		fclose(fp_r);
		fclose(fp_w);
	}
}