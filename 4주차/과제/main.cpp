#include "my_solver.h"

extern void program1_1(FILE* fp); // Newton-Raphson Method
extern void program1_2(FILE* fp); // Secant Method
extern void program1_3(FILE* fp); // Bisection Method

extern void sp_program1_1(FILE* fp); // Newton-Raphson Method
extern void sp_program1_2(FILE* fp); // Secant Method

double (*_f)(double);
double (*_fp)(double);

float (*_sp_f)(float);
float (*_sp_fp)(float);

int main() {
	FILE* fp;
	fp = fopen("result.txt", "w");

	_f = _f1;
	_fp = _fp1;

	printf("Problem 1-1, using f1\n");
	fprintf(fp, "Problem 1-1, using f1\n");
	program1_1(fp);
	program1_2(fp);

	_f = _f2;
	_fp = _fp2;

	printf("Problem 1-1, using f2\n");
	fprintf(fp, "Problem 1-1, using f2\n");
	program1_1(fp);
	program1_2(fp);

	_f = _f3;
	_fp = _fp3;

	printf("Problem 1-2\n");
	fprintf(fp,"Problem 1-2\n");

	for (int i = -0; i < 4; i++) 
		program1_1(fp);
	

	printf("Problem 1-4\n");
	fprintf(fp,"Problem 1-4\n");
	_f = _f4;
	_fp = _fp4;
	program1_1(fp);
	//program1_2(fp);

	_sp_f = _f4;
	_sp_fp = _fp4;
	sp_program1_1(fp);
	//sp_program1_2(fp);

	printf("Homework 1, using f1\n");
	fprintf(fp, "Homework 1, using f1\n");
	_f = _f1;
	program1_3(fp);

	printf("Homework 1, using f2\n");
	fprintf(fp, "Homework 1, using f2\n");
	_f = _f2;
	program1_3(fp);

	printf("Homework 1, using f3\n");
	fprintf(fp, "Homework 1, using f3\n");
	_f = _f3;
	program1_3(fp);


	_f = _f5;
	_fp = _fp5;
	printf("Homework 2, using f1\n");
	fprintf(fp, "Homework 2, using f1\n");
	program1_1(fp);
	
	fclose(fp);
}
