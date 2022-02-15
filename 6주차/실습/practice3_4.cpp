#include "my_solver.h"

#define SOLNUMS 3
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_4(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag)
{
	// origin function F(x)
	if (*iflag == 1) {
		/********************************/

		//x1+x2+x3-3=0
		//x1^2+x2^2+x3^2-5=0
		//e^x1+x1x2-x1x3-1=0
		fvec[0] = x[0] + x[1] + x[2] - 3.0;
		fvec[1] = x[0] * x[0] + x[1] * x[1] + x[2] * x[2] - 5.0;
		fvec[2] = exp(x[0]) + x[0] * x[1] - x[0] * x[2] - 1.0;





		/********************************/
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		/********************************/

		fjac[0] = 1.0;	fjac[1] = 2.0 * x[0];	fjac[2] = exp(x[0]) + x[1] - x[2];
		fjac[3] = 1.0;	fjac[4] = 2.0 * x[1];	fjac[5] = x[0];
		fjac[6] = 1.0;	fjac[7] = 2.0 * x[2];	fjac[8] = -x[0];






		/********************************/
	}
}

void practice3_4(void) {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.1, 1.2, 2.5 };	//need to initilize x0
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-4.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-4.txt");
		return;
	}

	/********************************/

	//for (int i = 0; i < 3; i++) scanf("%lf", &x[i]);

	hybrj1_(fcn3_4, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
	fprintf(fp_w, "\ninfo = %d\n", info);

	fprintf(fp_w, "root=");
	for (int i = 0; i < n; i++) fprintf(fp_w, "\t%.8lf", x[i]);
	fprintf(fp_w, "\nf(x)=");
	for (int i = 0; i < n; i++) fprintf(fp_w, "\t%.8lf", fvec[i]);
	fprintf(fp_w, "\n");
	/********************************/

	fclose(fp_w);
}
