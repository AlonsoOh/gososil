#include "my_solver.h"
#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

// f1 = x_1 + 10 * x_2 + 9;
// f2 = sqrt(5.0) * (x_3 - x_4) - 2 * sqrt(5.0);
// f3 = (x_2 - 2 * x_3)^2 - 9;
// f4 = sqrt(10.0)*(x_1 - x_4, 2)^2 - 2 * sqrt(10.0);
void fcn3_2(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag) {
	if (*iflag == 1) {
		fvec[0] = x[0] + (10 * x[1]) + 9;
		fvec[1] = sqrt(5.0) * (x[2] - x[3]) - 2 * sqrt(5.0);
		fvec[2] = (x[1] - (2 * x[2])) * (x[1] - (2 * x[2])) - 9;
		fvec[3] = sqrt(10.0) * (x[0] - x[3]) * (x[0] - x[3]) - 2 * sqrt(10.0);

	}

	else if (*iflag == 2) {
		fjac[0] = 1;
		fjac[4] = 10;
		fjac[8] = 0;
		fjac[12] = 0;

		fjac[1] = 0;
		fjac[5] = 0;
		fjac[9] = sqrt(5.0);
		fjac[13] = -sqrt(5.0);

		fjac[2] = 0;
		fjac[6] = 2 * (x[1] - (2 * x[2]));
		fjac[10] = -4 * (x[1] - (2 * x[2]));
		fjac[14] = 0;

		fjac[3] = 2 * sqrt(10.0) * (x[0] - x[3]);
		fjac[7] = 0;
		fjac[11] = 0;
		fjac[15] = -2 * sqrt(10.0) * (x[0] - x[3]);

	}
	return;
}

void program3_2() {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.9, -0.9, 1.25, -1.25 };	//need to initilize x0
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_found_3-2.txt", "w");
	
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_found_3-2.txt");
		return;
	}
	
	hybrj1_(fcn3_2, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
	fprintf(fp_w, "\ninfo = %d\n", info);
	fprintf(fp_w, "root=");
	
	for (int i = 0; i < n; i++) fprintf(fp_w, "\t%.8lf", x[i]);
	
	fprintf(fp_w, "\nf(x)=");
	
	for (int i = 0; i < n; i++) fprintf(fp_w, "\t%.8lf", fvec[i]);
	
	fprintf(fp_w, "\n");
}