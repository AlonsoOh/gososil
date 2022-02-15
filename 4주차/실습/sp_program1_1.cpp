#include "my_solver.h"

extern float (*_sp_f)(float);
extern float (*_sp_fp)(float);

/*********************************************
  Newton-Rapson Method
**********************************************/
void sp_program1_1(FILE* fp) {
	float x0;
	int i = 0;

	printf("\n=======Newton-Raphson Method=======\n\n");
	fprintf(fp, "\n=======Newton-Raphson Method=======\n\n");
	printf("Enter the x0: ");
	scanf("%f", &x0);

	printf("i\t\txn1\t\t\t\t|f(xn1)|\t\t\t|f'(xn1)|\n");
	fprintf(fp, "i\t\txn1\t\t\t\t|f(xn1)|\t\t\t|f'(xn1)|\n");
	printf("%d\t%20.18e\t", i, x0);
	fprintf(fp, "%d\t%20.18e\t", i, x0); i++;
	printf("%20.18e\t%20.18e\n", fabs(_sp_f(x0)), fabs(_sp_fp(x0)));
	fprintf(fp, "%20.18e\t%20.18e\n", fabs(_sp_f(x0)), fabs(_sp_fp(x0)));

	while (1) {
		float x1 = x0 - _sp_f(x0) / _sp_fp(x0);
		printf("%d\t%20.18e\t", i, x1);
		fprintf(fp, "%d\t%20.18e\t", i, x1);
		i++;

		printf("%20.18e\t%20.18e\n", fabs(_sp_f(x1)), fabs(_sp_fp(x1)));
		fprintf(fp, "%20.18e\t%20.18e\n", fabs(_sp_f(x1)), fabs(_sp_fp(x1)));

		if (fabs(_sp_f(x1)) < DELTA) break;
		if (i >= Nmax) break;
		if (fabs(x1 - x0) < EPSILON) break;

		x0 = x1;
	}
	printf("\n");
	fprintf(fp, "\n");
}
