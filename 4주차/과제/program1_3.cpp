#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Bisection Method
**********************************************/
void program1_3(FILE* fp) {
	double a0, b0;
	double mid, before;
	int i = 0;

	printf("\n=======Bisection Method=======\n\n");
	fprintf(fp, "\n=======Bisection Method=======\n\n");
	printf("Enter the a0, b0: ");

	while (1) {
		scanf("%lf %lf", &a0, &b0);
	
		if (_f(a0) * _f(b0) < 0) break;
		
		else printf("please enter proper a0, b0\n");
	}

	mid = (a0 + b0) / 2;
	printf("i\t\txn1\t\t\t\tan1\t\t\t\tbn1\t\t\t\t|f(xn1)|\n\n");
	printf("%d\t%20.18e\t%20.18e\t%20.18e\t", i, mid, a0, b0);
	printf("%20.18e\n", fabs(_f(mid)));
	fprintf(fp,"i\t\txn1\t\t\t\tan1\t\t\t\tbn1\t\t\t\t|f(xn1)|\n\n");
	fprintf(fp,"%d\t%20.18e\t%20.18e\t%20.18e\t", i, mid, a0, b0);
	fprintf(fp, "%20.18e\n", fabs(_f(mid))); i++;

	while (1) {
		if (_f(mid) < 0) {
			if (_f(a0) < 0) a0 = mid;
			
			else b0 = mid;
		}

		else {
			
			if (_f(a0) < 0) b0 = mid;
			
			else  a0 = mid;
		}

		double before = mid;
		
		mid = (a0 + b0) / 2;
		printf("%d\t%20.18e\t%20.18e\t%20.18e\t", i, mid, a0, b0);
		printf("%20.18e\n", fabs(_f(mid)));
		fprintf(fp,"%d\t%20.18e\t%20.18e\t%20.18e\t", i, mid, a0, b0);
		fprintf(fp,"%20.18e\n", fabs(_f(mid)));
		i++;

		if (fabs(_f(mid)) < DELTA) break;
		
		if (i >= Nmax) break;
		
		if (fabs(mid - before) < EPSILON) break;	
	}

	printf("\n");
	fprintf(fp, "\n");
}
