#include "my_solver.h"

#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_6(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag) {
	// origin function F(x)
	if (*iflag == 1) {
		/********************************/


		fvec[0] = 3.0 * x[0] * x[0] - 2.0 * x[1] * x[1] - 1.0;
		fvec[1] = x[0] * x[0] - 2.0 * x[0] + x[1] * x[1] + 2.0 * x[1] - 8.0;






		/********************************/
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		/********************************/


		fjac[0] = 6.0 * x[0];	fjac[1] = 2.0 * x[0] - 2.0;
		fjac[2] = -4.0 * x[1];	fjac[3] = 2.0 * x[1] + 2.0;






		/********************************/
	}
}
const int dx[] = { -1, -1, 1, 1 };
const int dy[] = { -1, 1, -1, 1 };
void practice3_6(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = { -4.0, 5.0 };	//need to initilize x0
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-6.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-6.txt");
		return;
	}

	/********************************/
	for (int i = 0; i < 4; i++) {
		double y[] = { x[0] * dx[i], x[1] * dy[i] };
		hybrj1_(fcn3_6, &n, y, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
		fprintf(fp_w, "\ninfo = %d\n", info);

		fprintf(fp_w, "root=");
		for (int i = 0; i < n; i++) fprintf(fp_w, "\t%.8lf", y[i]);
		fprintf(fp_w, "\nf(x)=");
		for (int i = 0; i < n; i++) fprintf(fp_w, "\t%.8lf", fvec[i]);
		fprintf(fp_w, "\n");
		fprintf(fp_w, "\n");
	}



	/********************************/

	fclose(fp_w);
}