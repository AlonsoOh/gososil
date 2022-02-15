#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Newton-Rapson Method
**********************************************/
void program1_1(FILE* fp) {
	double x0;
	int i = 0;

	printf("\n=======Newton-Raphson Method=======\n\n");
	fprintf(fp,"\n=======Newton-Raphson Method=======\n\n");
	printf("Enter the x0: ");
	scanf("%lf", &x0);

	printf("i\t\txn1\t\t\t\t|f(xn1)|\t\t\t|f'(xn1)|\n");
	fprintf(fp,"i\t\txn1\t\t\t\t|f(xn1)|\t\t\t|f'(xn1)|\n");
	printf("%d\t%20.18e\t", i, x0); 
	fprintf(fp, "%d\t%20.18e\t", i, x0); i++;
	printf("%20.18e\t%20.18e\n", fabs(_f(x0)), fabs(_fp(x0)));
	fprintf(fp, "%20.18e\t%20.18e\n", fabs(_f(x0)), fabs(_fp(x0)));

	while (1) {
		double x1 = x0 - _f(x0) / _fp(x0);

		printf("%d\t%20.18e\t",i,x1);
		fprintf(fp, "%d\t%20.18e\t", i, x1);
		i++;
		printf("%20.18e\t%20.18e\n", fabs(_f(x1)), fabs(_fp(x1)));
		fprintf(fp,"%20.18e\t%20.18e\n", fabs(_f(x1)), fabs(_fp(x1)));

		if (fabs(_f(x1)) < DELTA) break;
		
		if (i >= Nmax) break;
		
		if (fabs(x1 - x0) < EPSILON) break;

		x0 = x1;
	}

	printf("\n");
	fprintf(fp, "\n");
}
