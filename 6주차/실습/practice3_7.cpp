#include "my_solver.h"

#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_7(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/

	fvec[0] = 2.0 * x[0] * x[0] * x[0] - 12.0 * x[0] - x[1] - 1.0;
	fvec[1] = 3.0 * x[1] * x[1] - 6.0 * x[1] - x[0] - 3.0;







	/********************************/
}
const int dx[] = { -1, -1, 1, 1 };
const int dy[] = { -1, 1, -1, 1 };
using ii = pair<double, double>;
struct P {
	ii xy, fval;
	bool operator <(P& rhs) {
		if (xy.first != rhs.xy.first) return xy.first < rhs.xy.first;
		return xy.second < rhs.xy.second;
	}
};
void practice3_7(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = { 1.0, 0.0 };	//need to initilize x0
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-7.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-7.txt");
		return;
	}

	/********************************/
	std::vector<ii> v;
	std::vector<P> ret, vv;
	for (int i = -4; i < 4; i++)
		for (int j = -5; j < 5; j++)
			v.push_back({ i,j });
	for (int i = 0; i < (int)v.size(); i++) {
		double y[] = { v[i].first, v[i].second };
		hybrd1_(fcn3_7, &n, y, fvec, &tol, &info, wa, &lwa);
		if (info == 1) ret.push_back({{ y[0], y[1] }, { fvec[0], fvec[1] }});
	}
	std::sort(ret.begin(), ret.end());
	for (int i = 0; i < (int)ret.size()-1; i++) {
		if (abs(ret[i].xy.first - ret[i + 1].xy.first) > tol)
			vv.push_back(ret[i]);
	}
	if (abs(ret.back().xy.first - v.back().first) > tol) vv.push_back(ret.back());
	//printf("%d\n", vv.size());
	for (auto& it : vv) 
		fprintf(fp_w, "x = %.8lf\ty = %.8lf\nf1(x,y) = %.8lf\tf2(x,y) = %.8lf\n", it.xy.first, it.xy.second, it.fval.first, it.fval.second);
	/********************************/

	fclose(fp_w);
}