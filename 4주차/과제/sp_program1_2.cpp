#include "my_solver.h"

extern float (*_sp_f)(float);
extern float (*_sp_fp)(float);

/*********************************************
  Secant Method
**********************************************/
void sp_program1_2(FILE* fp) {
	float x0, x1;
	int i = 0;

	printf("\n=======Secant Method=======\n\n");
	fprintf(fp, "\n=======Secant Method=======\n\n");
	printf("Enter the x0, x1: ");
	scanf("%f %f", &x0, &x1);
	printf("i\t\txn1\t\t\t\txn0\t\t\t\t|f(xn1)|\t\t\t|f(xn0)|\n\n");
	printf("%d\t%20.18e\t%20.18e\t", i, x1, x0);
	printf("%20.18e\t%20.18e\n", fabs(_sp_f(x1)), fabs(_sp_f(x0)));
	fprintf(fp, "i\t\txn1\t\t\t\txn0\t\t\t\t|f(xn1)|\t\t\t|f(xn0)|\n");
	fprintf(fp, "%d\t%20.18e\t%20.18e\t", i, x1, x0); i++;
	fprintf(fp, "%20.18e\t%20.18e\n", fabs(_sp_f(x1)), fabs(_sp_f(x0)));

	while (1) {
		float x2 = x1 - _sp_f(x1) * ((x1 - x0) / (_sp_f(x1) - _sp_f(x0)));
		
		printf("%d\t%20.18e\t%20.18e\t", i, x2, x1);
		printf("%20.18e\t%20.18e\n", fabs(_sp_f(x2)), fabs(_sp_f(x1)));
		fprintf(fp, "%d\t%20.18e\t%20.18e\t", i, x2, x1);
		fprintf(fp, "%20.18e\t%20.18e\n", fabs(_sp_f(x2)), fabs(_sp_f(x1)));
		i++;

		if (fabs(_sp_f(x2)) < DELTA) break;
		
		if (i >= Nmax) break;
		
		if (fabs(x2 - x1) < EPSILON) break;

		x0 = x1;
		x1 = x2;
	}

	printf("\n");
	fprintf(fp, "\n");
}
