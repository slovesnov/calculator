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
 * TYPE 3 - sample3
 * TYPE 4 - sample4 random() & threads
 * else   - sample5
 */
#define TYPE 0

#if TYPE>1
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
	try {
		double v = ExpressionEstimator::calculate("sin(pi/4)");
		std::cout << v;
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

#elif TYPE==2

int main() {
	try {
		ExpressionEstimator estimator;
		estimator.compile("x0+2*x1");

		std::cout << estimator.calculate(0, 1) << std::endl;

		std::cout << estimator.calculate( { 2, 3 }) << std::endl;

		const std::vector<double> v = { 1, 2 };
		std::cout << estimator.calculate(v) << std::endl;

		const int args = 2;
		double vv[args] = { 3, 6 };
		std::cout << estimator.calculate(vv, args) << std::endl;

		ExpressionEstimator estimator1("x0+2*sin(pi*x1)");
		std::cout << estimator1.calculate(2, 1. / 6) << std::endl;

	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

#elif TYPE==3
/*
"sin(pi/4)" = 0.707107
"1+2+" = unexpected operator
"3*x0+2*x1*x0"[3.000000, 6.000000] = 45.000000
"6*x0"[8.000000, 6.000000] = invalid number of expression arguments
 */

int main() {
	ExpressionEstimator estimator;
	int i;
	const std::string expression[] = { "sin(pi/4)", "1+2+" };
	for (auto &a : expression) {
		std::cout <<"\""<< a<<"\" = ";
		try {
			std::cout << ExpressionEstimator::calculate(a) << std::endl;
		} catch (std::exception &e) {
			std::cout << e.what() << std::endl;
		}
	}

	const char *expression1[] = { "3*x0+2*x1*x0", "6*x0" };
	const std::vector<std::vector<double>> vv = { { 3, 6 }, { 8, 6 } };
	i = 0;
	for (auto &a : expression1) {
		std::cout << "\"" << a << "\"" << vectorToString(vv[i])<< " = ";
		try {
			estimator.compile(a);
			std::cout << estimator.calculate(vv[i]) << std::endl;
		} catch (std::exception &e) {
			std::cout << e.what() << std::endl;
		}
		i++;
	}
}

#elif TYPE==4

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
//		estimator.compile("a+ 2*A +3*X0+x0",{"a","A","X0","x0"} );
//		std::cout<<estimator.calculate(1,0,3,4)<< std::endl;
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

		char p[]="x0+2*x1";
		std::vector<std::string> v={"a","b","a"};//"x0","x1","a"
		ExpressionEstimator es(p, v );
		std::cout<<"arguments"<<es.getArguments()<< std::endl;
		//int i=5,j=4;
		std::cout<<es.calculate(1,2,3)<< std::endl;
	}
	catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}

}
#endif
