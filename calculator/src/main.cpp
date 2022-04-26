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
 * TYPE 3 - parser check using test cases
 * TYPE 4 - sample random() & threads
 * else   - sample
 */
#define TYPE 3

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

#include <vector>
#include <fstream>

#include "testcase/Case.h"

#include "aslov.h"

int main() {
	const std::string fn = "testcases.txt";
	std::ifstream infile(fn);
	if(!infile.is_open()){//not open if run using shortcut
		printl("need to run under eclipse");
		return 0;
	}
	std::string s, q[3];
	int i, j, k, r[] = { 0, 0 }, li[3],line;
	TestCase::Case t;

	i = 0;
	line = 1;
	while (std::getline(infile, s)) {
		if (s.empty()) {
			line++;
			continue;
		}
		q[i % 3] = s;
		li[i%3]=line;
		if (i % 3 == 2) {
			t.set(q, li);
			r[t.test()]++;
		}
		i++;
		line++;
	}

	j = r[0] + r[1];
	k = log10(j) + 1;
	for (i = 1; i >= 0; i--) {
		println("%-5s %*d/%d=%5.1lf%%", i ? "ok" : "error", k, r[i], j,
				r[i] * 100. / j);
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
//	const std::string data[]={"0b10.11+1","0b.1-1","0b1.-1","0b.","0b.*2"};
//	const double error=std::numeric_limits<double>::max();
//	double r[]={3.75,-.5,0,error};

	try{
//		std::cout<<ExpressionEstimator::calculate("0b10.11+1")<< std::endl;//3.75
//		std::cout<<ExpressionEstimator::calculate("0b.1-1")<< std::endl;//-0.5
//		std::cout<<ExpressionEstimator::calculate("0b1.-1")<< std::endl;//0
//		std::cout<<ExpressionEstimator::calculate("0b.01*2")<< std::endl;
		std::cout<<ExpressionEstimator::calculate("0b0.3*2")<< std::endl;
	}
	catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}

}
#endif
