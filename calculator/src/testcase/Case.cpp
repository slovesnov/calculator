/*
 * Case.cpp
 *
 *  Created on: 24.04.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#include <cassert>

#include "aslov.h"
#include "Case.h"

namespace TestCase {

void Case::set(std::string s[],int lines[]) {
	data.set(s[0],lines[0]);
	action.set(s[1],lines[1]);
	result.set(s[2],lines[2]);
}

bool Case::test() {
	Result r;
	double v;
	if(action.action==Action::ActionEnum::STATIC_COMPILE){
		try {
			v = ExpressionEstimator::calculate(data.compile);
			r.set(Result::OK, v);
		} catch (std::exception &ex) {
			r.set(Result::COMPILE_ERROR);
		}
	}
	else if(action.action==Action::ActionEnum::COMPILE_CALCULATE){
		try {
			e.compile(data.compile,data.variables);
			try {
				v = e.calculate(data.values);
				r.set(Result::OK, v);
			} catch (std::exception &ex) {
				r.set(Result::CALCULATE_ERROR);
			}
		} catch (std::exception &ex) {
			r.set(Result::COMPILE_ERROR);
		}
	}
	else{
		assert(0);
		return false;
	}

	bool b=r==result;
	if(!b){
		//r what we got here,result is data from file
		printl(data,r,"file",result);
	}
	return b;
}

} /* namespace TestCase */
