#include "my_solver.h"

#define SOLNUMS 3
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_3(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag)
{
	// origin function F(x)
	if (*iflag == 1) {
		/********************************/

		fvec[0] = exp(2 * x[0]) - x[1] + 4.0;
		fvec[1] = x[1] - x[2]* x[2] - 1;
		fvec[2] = x[2] - sin(x[0]);








		/********************************/
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		/********************************/


		fjac[0] = 2.0 * exp(2.0 * x[0]);	fjac[1] = 0.0;					fjac[2] = -cos(x[0]);
		fjac[3] = -1.0;						fjac[4] = 1.0;					fjac[5] = 0.0;
		fjac[6] = 0.0;						fjac[7] = -2.0*x[2];			fjac[8] = 1.0;







		/********************************/
	}
}

void practice3_3(void) {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.0, 0.0, 0.0 };	//need to initilize x0
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-3.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-3.txt");
		return;
	}

	/********************************/

	//for (int i = 0; i < 3; i++) scanf("%lf", &x[i]);
	
	hybrj1_(fcn3_3, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
	fprintf(fp_w, "\ninfo = %d\n", info);

	fprintf(fp_w, "root=");
	for (int i = 0; i < n; i++) fprintf(fp_w, "\t%.8lf", x[i]);
	fprintf(fp_w, "\nf(x)=");
	for (int i = 0; i < n; i++) fprintf(fp_w, "\t%.8lf", fvec[i]);
	fprintf(fp_w, "\n");






	/********************************/

	fclose(fp_w);
}
