/*
 * Case.h
 *
 *  Created on: 24.04.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#ifndef TESTCASE_CASE_H_
#define TESTCASE_CASE_H_

#include "Data.h"
#include "Action.h"
#include "Result.h"
#include "../estimator/expressionEstimator.h"

namespace TestCase {

class Case {
public:
	Data data;
	Action action;
	Result result;
	ExpressionEstimator e;
	Case();
	virtual ~Case();
	void set(std::string s[],int lines[]);
	bool test();
};

} /* namespace TestCase */

#endif /* TESTCASE_CASE_H_ */
