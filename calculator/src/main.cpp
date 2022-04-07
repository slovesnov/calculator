/******************************************************
 Copyright (c/c++) 2013-doomsday by Aleksey Slovesnov 
 homepage http://slovesnov.users.sourceforge.net/?parser
 email slovesnov@yandex.ru
 All rights reserved.
 ******************************************************/

/*
 * TYPE 0 - calculator (under GTK+)
 * TYPE 1 - sample1
 * TYPE 2 - sample2
 * TYPE 3 - sample3
 * else   - sample4
 */
#define TYPE 0

#include "expressionEstimator.h"
#include <cstdio>
#if TYPE==0
//only for TYPE==0
#include "aslov.h"
#include "CalculatorWindow.h"

int main(int argc, char *argv[]) {
	gtk_init (&argc, &argv);
	aslovInit(argv);
	CalculatorWindow c;
}

#elif TYPE==1

int main() {
	try {
		double v = ExpressionEstimator::calculate("sin(pi/4)");
		printf("%lf", v);
	} catch (Exception &e) {
		printf("%s\n", e.what());
	}
}

#elif TYPE==2

int main() {
	try {
		ExpressionEstimator estimator;
		estimator.compile("x0+2*x1");

		printf("%lf\n", estimator.calculate( { 2, 3 }));

		std::vector<double> v = { 1, 2 };
		printf("%lf\n", estimator.calculate(v));

		const int arguments = 2;
		const double values[][arguments] = { { 3, 6 }, { 8, 6 } };
		const int valuesSize = sizeof(values) / sizeof(values[0]);
		for (int i = 0; i < valuesSize; i++) {
			printf("%lf\n", estimator.calculate(values[i], arguments));
		}

	} catch (Exception &e) {
		printf("%s\n", e.what());
	}
}

#elif TYPE==3

std::string arraytoString(const double *v, const int arguments) {
	std::string s = "[";
	char buff[1024];
	for (int i = 0; i < arguments; i++) {
		sprintf(buff, "%lf", v[i]);
		s += buff;
		s += i == arguments - 1 ? "]" : ", ";
	}
	return s;
}

int main() {
	ExpressionEstimator estimator;
	int i;
	const char *s;
	const char *expression[] = { "sin(pi/4)", "1+2+" };
	const int expressionSize = sizeof(expression) / sizeof(expression[0]);
	for (i = 0; i < expressionSize; i++) {
		s = expression[i];
		printf("\"%s\" = ", s);
		try {
			printf("%lf\n", ExpressionEstimator::calculate(s));
		} catch (Exception &e) {
			printf("%s\n", e.what());
		}
	}

	const char *expression1[] = { "3*x0+2*x1*x0", "6*x0" };

	const int arguments = 2;
	const double values[][arguments] = { { 3, 6 }, { 8, 6 } };
	const int valuesSize = sizeof(values) / sizeof(values[0]);

	const double *v;
	for (i = 0; i < valuesSize; i++) {
		v = values[i];
		s = expression1[i];
		printf("\"%s\"%s = ", s, arraytoString(v, arguments).c_str());
		try {
			estimator.compile(s);
			printf("%lf\n", estimator.calculate(v, arguments));
		} catch (Exception &e) {
			printf("%s\n", e.what());
		}
	}
}
#else

std::string arraytoString(const double *v, const int arguments) {
	std::string s = "[";
	char buff[1024];
	for (int i = 0; i < arguments; i++) {
		sprintf(buff, "%lf", v[i]);
		s += buff;
		s += i == arguments - 1 ? "]" : ", ";
	}
	return s;
}

#include <thread>

void f(int t) {
	for(int i=0;i<3;i++){
	printf("t%d %lf\n",t, ExpressionEstimator::calculate("random()"));
	}
}

int main() {
	int cores=3;
	int i;
	std::vector<std::thread> vt;

	for (i = 0; i < cores; ++i) {
		vt.push_back(std::thread(f, i));
	}

	for (auto& a : vt){
		a.join();
	}
	printf("\n");

	ExpressionEstimator estimator;
	const int arguments = 2;
	const double v[arguments] = { 3, 6 };

	for(i=0;i<3;i++){
		printf("%lf\n", ExpressionEstimator::calculate("random()"));
	}

	printf("\n");
	estimator.compile("random()+x0+x1");

	try {
		for(i=0;i<3;i++){
			printf("%lf\n", estimator.calculate(v, arguments));
		}
	} catch (Exception &e) {
		printf("%s\n", e.what());
	}

}

#endif
