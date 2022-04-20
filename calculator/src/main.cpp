/******************************************************
 Copyright (c/c++) 2013-doomsday by Aleksey Slovesnov 
 homepage http://slovesnov.users.sourceforge.net/?parser
 email slovesnov@yandex.ru
 All rights reserved.
 ******************************************************/

/*
 * TYPE -1 - calculator (under GTK+)
 * TYPE 0 - testFormWindow (under GTK+)
 * TYPE 1 - sample1
 * TYPE 2 - sample2
 * TYPE 3 - sample3 random() & threads
 * else   - sample4
 */
#define TYPE 2

#if TYPE>0
#include <iostream>
#include <sstream>

#include "estimator/expressionEstimator.h"

std::string vectorToString(const std::vector<double> &v) {
	std::stringstream ss;
	int i = 0;
	for (auto &a : v) {
		ss << (i ? ", " : "[") << a;
		i++;
	}
	ss << "]";
	return ss.str();
}
#endif

#if TYPE==-1
#include "CalculatorWindow.h"

int main(int argc, char *argv[]) {
	gtk_init (&argc, &argv);
	aslovInit(argv);
	CalculatorWindow c;
}

#elif TYPE==0
#include "TestFormWindow.h"

int main(int argc, char *argv[]) {
	gtk_init (&argc, &argv);
	aslovInit(argv);
	TestFormWindow c;
}

#elif TYPE==1

int main() {
	double v;
	try {
		v = ExpressionEstimator::calculate("sin(pi/4)");
		std::cout << v << std::endl;
		v = ExpressionEstimator::calculate("pow( sin(pi/10)*4+1 , 2)");
		std::cout << v << std::endl;
	} catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}

#elif TYPE==2

int main() {
	ExpressionEstimator e;
	double v;
	std::vector<double> d;
	std::vector<std::string> s;
	try {
		e.compile("x0+2*sin(pi*x1)");
		v = e.calculate(1, .25);
		std::cout << v << std::endl;
		d = { 1, 1. / 6 };
		v = e.calculate(d);
		std::cout << v << std::endl;
		e.compile("x0+2*X1"); // case insensitive variable names
		v = e.calculate( { 1, .25 });
		std::cout << v << std::endl;

		ExpressionEstimator e1("x0+2*x1");
		const double f[] = { 1, 3 };
		v = e1.calculate(f, 2);
		std::cout << v << std::endl;

		e.compile("a+2*b", "a", "b"); // case sensitive variable names
		// or e.compile("a+2*b", std::vector<std::string> { "a", "b" });
		// or e.compile("a+2*b", { "a", "b" });
		// or std::string s[]={ "a", "b" }; e.compile("a+2*b", s, 2);
		v = e.calculate(1, 1);
		std::cout << v << std::endl;

		ExpressionEstimator e2("a+2*A", "a", "A");// case sensitive variable names
		// or ExpressionEstimator e2("a+2*A", std::vector<std::string> { "a", "A" });
		// or ExpressionEstimator e2("a+2*A", { "a", "A" });
		// or std::string s[] = { "a", "A" }; ExpressionEstimator e2("a+2*A", s, 2);
		v = e2.calculate(1, 2);
		std::cout << v << std::endl;

	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

#elif TYPE==3

#include <thread>

void f(int t) {
	for(int i=0;i<3;i++){
	printf("t%d %lf\n",t, ExpressionEstimator::calculate("random()"));
	}
}

int main() {
	int cores=3;
	int i,j;
	std::vector<std::thread> vt;

	for (i = 0; i < cores; ++i) {
		vt.push_back(std::thread(f, i));
	}

	for (auto& a : vt){
		a.join();
	}
	printf("\n");

	const int arguments = 2;
	const double v[arguments] = { 3, 6 };

	for(i=0;i<3;i++){
		printf("%lf\n", ExpressionEstimator::calculate("random()"));
	}

	printf("\n");
	ExpressionEstimator estimator[2];
	for(j=0;j<2;j++){
		estimator[j].compile("random()+x0+x1");
	}

	try {
		for(j=0;j<2;j++){
			for(i=0;i<3;i++){
				printf("%d %d %lf\n",i,j, estimator[j].calculate(v, arguments));
			}
		}
	} catch (std::exception &e) {
		printf("%s\n", e.what());
	}

}
#else
int main(){
	ExpressionEstimator estimator;
//	int i;
//	const char *s;

	try{
		double v=-1;
		estimator.compile("a+ 2*A +3*X0+x0",{"a","A","X0","x0"} );
		std::cout<<estimator.calculate(1,0,3,4)<< std::endl;
//
//		std::vector<std::string> v={"A","a"};
//		estimator.compile("a+ 2*A +5",v );
//		std::cout<<estimator.calculate(1,2)<< std::endl;
//
//		estimator.compile("a+ sin(b)","b","a" );
//		std::cout<<estimator.calculate(M_PI/6,1)<< std::endl;
//
//		std::string vv[]={"A","a"};
//		estimator.compile("a+ 2*A",vv,2 );
//		std::cout<<estimator.calculate(1,2)<< std::endl;

	}
	catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}

}
#endif
