#define _USE_MATH_DEFINES
#include <math.h>

float _f1(float x) {
	return x * x - 4 * x + 4 - log(x);

}

float _fp1(float x) {
	return 2 * x - 4 - (1 / x);
}

float _f2(float x) {
	return x + 1 - 2 * sin(M_PI * x);
}

float _fp2(float x) {
	return 1 - 2 * M_PI * cos(M_PI * x);
}

float _f3(float x) {
	return x * x * x * x - 11.0 * x * x * x + 42.35 * x * x - 66.55 * x + 35.1384;
}

float _fp3(float x) {
	return 4 * x * x * x - 3 * 11.0 * x * x + 42.35 * 2 * x - 66.55;
}

float _f4(float x) {
	return log(x) - 1;
}

float _fp4(float x) {
	return 1 / x;
}