#include "my_solver.h"

extern void program1_1(FILE* fp); // Newton-Raphson Method
extern void program1_2(FILE* fp); // Secant Method

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
	for (int i = -0; i < 4; i++) {
		program1_1(fp);
	}

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

	fclose(fp);
}
